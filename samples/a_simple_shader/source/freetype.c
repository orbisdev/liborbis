/*
 * freetype example1.c
 * liborbis port, 2018
 * masterzorag@gmail.com
 */

#include <ps4link.h>
#include <math.h>       // sinf, cosf
#include <sys/fcntl.h>  // O_RDONLY
#include <orbis2d.h>
extern Orbis2dConfig *orbconf;  // hook main 2d conf to get framebuffer address
static uint32_t __attribute__((aligned(16))) *pixel = NULL;

///TTF
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library library;
static FT_Face    face = NULL;
static FT_Byte *buffer = NULL;  // stores ttf font data
static size_t   bufsize;

/* precomputed gradient colors [0-7] */
#define SIMPLE_GRADIENT_STEPS 8
/*static uint32_t fading_color[8] = { 0xFFFF0000, 0xFF0000FF, 0xFFFF0000, 0xFF0000FF,
                                      0xFFFF0000, 0xFF0000FF, 0xFFFF0000, 0xFF0000FF };*/
static uint32_t fading_color[SIMPLE_GRADIENT_STEPS];  // precomputed gradient [0-7]

static uint32_t c1 = 0xFFFF0000,
                c2 = 0xFF0000FF;

/***********************************************************************
* simple gradient (ARGB)
*
* uint32_t *a   = pointer to start color
* uint32_t *b   = pointer to end color
* uint8_t steps = number of steps we split fade
* uint8_t step  = which step we compute and return
***********************************************************************/
typedef float float4 __attribute__((ext_vector_type(4)));

static uint32_t simple_gradient(const uint32_t *a, const uint32_t *b, const uint8_t steps, const uint8_t step)
{
    float4  fr4, to4, col;  // use .xyzw vector components as ARGB color components

    // load vectors
    fr4 = (float4){ GET_A(*a), GET_R(*a), GET_G(*a), GET_B(*a) };
    to4 = (float4){ GET_A(*b), GET_R(*b), GET_G(*b), GET_B(*b) };

    // compute which color we return
    col = ((to4 - fr4) / ( (float)(steps -1) ));  // num of slices

    fr4 += col * ( (float)(step) );  // requested slice

    return ARGB( (uint8_t)fr4.x, (uint8_t)fr4.y, (uint8_t)fr4.z, (uint8_t)fr4.w );
}


/***********************************************************************
* update_gradient
*
* precompute palette to use in print_text() and setup colors range
*
* uint32_t *a = pointer to start color
* uint32_t *b = pointer to end color
***********************************************************************/
void FT_update_gradient(const uint32_t *a, const uint32_t *b)
{
    for(uint8_t i = 0; i < SIMPLE_GRADIENT_STEPS; i++)
    {
        fading_color[i] = (*a == *b) ? *a : simple_gradient(a, b, SIMPLE_GRADIENT_STEPS, i);
    }
}


// can be a default wrapper!
// returned buffer must be free(): actually in FT_end()
static uint8_t *buffer_from_host(const char *path, size_t *ret_size)
{
	int fd;             // descriptor to manage file from host0
	int filesize;       // variable to control file size
	uint8_t *buf=NULL;  // buffer for read from host0 file
    *ret_size = 0;      // each time we refresh it

	// we open file in read only from host0 ps4sh include the full path with host0:/.......
	fd=ps4LinkOpen(path,O_RDONLY,0);

	if(fd<0)  //If we can't open file from host0 print  the error and return
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkOpen returned error %d\n",fd);
		return NULL;
	}
	filesize=ps4LinkLseek(fd,0,SEEK_END);  // Seek to final to get file size
	if(filesize<0)                         // If we get an error print it and return
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkSeek returned error %d\n",fd);
		ps4LinkClose(fd);
		return NULL;
	}

	ps4LinkLseek(fd,0,SEEK_SET);  //Seek back to start
	buf=malloc(filesize);         //Reserve  memory for read buffer

	int numread=ps4LinkRead(fd,buf,filesize);  //Read filsesize bytes to buf
	ps4LinkClose(fd);  //Close file

	if(numread!=filesize)                      //if we don't get filesize bytes we are in trouble
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkRead returned error %d\n",numread);
		return NULL;
	}

	*ret_size = filesize;
	return buf;
}

/* internally draw via liborbis2d */
static void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y) // glyph refs
{
    //uint32_t pixelColor = 0xFF000000;
    uint8_t c;

    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for(i = x, p = 0; i < x_max; i++, p++ )
    {
        if(i < 0 || i >= ATTR_WIDTH) continue;

        for(j = y, q = 0; j < y_max; j++, q++ )
        {
            if(j < 0 || j >= ATTR_HEIGHT ) continue;

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            c = bitmap->buffer[q * bitmap->width + p];

            /* we can do better about color */
            if(c > 0
            && c < 0x80) {
                //pixelColor = ARGB(0xFF, c, c, c);
                //orbis2dDrawPixelColor(i, j, 0xFF808080);
                pixel[j * ATTR_WIDTH + i] = 0xFF808080;
            } else if(c > 0x80) {
                //pixelColor = ARGB(0xFF, 0x10 + c + q*2, 0x10 + c + q*2, 0x10 + c + q*2);
                //orbis2dDrawPixelColor(i, j, fading_color[q /3]);
                pixel[j * ATTR_WIDTH + i] = fading_color[q /3];
            }
        }
    }
}

#define ANGLE_START  0.0
static float angle = ANGLE_START;

// change angle from main()
void add_angle(void)
{
    float a = angle;
    a += 0.25;
    if(a >= 360) a = 0;
    if(a < 0)    a = 360;
    angle = a;
    //debugNetPrintf(DEBUG,"angle: %.f\n", angle);
}

/* new glyph sequence loader */
#include <pthread.h>
pthread_mutex_t lock;

// print from main() or some pthread
void FT_print_text(uint dst_x, uint dst_y, const char *text)
{
    if(!face) return;

    pthread_mutex_lock(&lock);

    int num_chars = strlen(text);
    FT_GlyphSlot  slot = face->glyph;   /* a small shortcut */
    FT_Vector     pen;                  /* untransformed origin */

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (dst_x, dst_y) relative to the upper left corner  */
    pen.x = dst_x * 64;
    pen.y = ( ATTR_HEIGHT - dst_y ) * 64;

    /* transformation matrix */
    FT_Matrix matrix;
    float     a = ( angle / 360 ) * 3.14159 * 2;  // use of degrees
    /* set up matrix */
    matrix.xx = (FT_Fixed)( cosf( a ) * 0x10000L );
    matrix.xy = (FT_Fixed)(-sinf( a ) * 0x10000L );
    matrix.yx = (FT_Fixed)( sinf( a ) * 0x10000L );
    matrix.yy = (FT_Fixed)( cosf( a ) * 0x10000L );

    /* refresh current fb address */
    pixel = (uint32_t *)orbconf->surfaceAddr[orbconf->currentBuffer];

    for(uint n = 0; n < num_chars; n++)
    {
        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );

        /* load glyph image into the slot (erase previous one) */
        FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        /* ignore errors */

        /* now, draw to our target surface (convert position) */
        draw_bitmap(&slot->bitmap,
                     slot->bitmap_left,
                     ATTR_HEIGHT - slot->bitmap_top);

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
        //debugNetPrintf(DEBUG,"[%d] %d %d %d %d\n", n, slot->bitmap_left, slot->bitmap_top, pen.x /64, pen.y /64);
    }
    pthread_mutex_unlock(&lock);
}


// init freetype
/*... initialize library ...
... create face object ...
... set character size ...*/
void FT_init()
{
    FT_update_gradient(&c1, &c2);

    FT_Error error;

    /* initialize library */
    error = FT_Init_FreeType( &library );
    /* error handling omitted */
    debugNetPrintf(DEBUG,"FT_Init_FreeType return:\t%d, library at %p\n", error, library);

    /* load default font */
    if(!buffer) buffer = buffer_from_host("host0:main.ttf", &bufsize);

    if(!buffer) return;
    // passed, report
    debugNetPrintf(DEBUG,"data buffer at %p, size %u\n", buffer, bufsize);

    /* create face object */
    if(!face)
    {
        error = FT_New_Memory_Face( library,
                                    buffer,    /* first byte in memory */
                                    bufsize,   /* size in bytes        */
                                    0,         /* face_index           */
                                    &face );
        /* error handling omitted */
        debugNetPrintf(DEBUG,"FT_New_Memory_Face return:\t%d, face at:\t%p\n", error, face);
    }

    /* use 14pt at 100dpi */
    error = FT_Set_Char_Size( face, 14 * 64, 0, 100, 0 );    /* set character size */
    /* error handling omitted */
    debugNetPrintf(DEBUG,"FT_Set_Char_Size return:\t%d\n", error);

    /* cmap selection omitted;                                        */
    /* for simplicity we assume that the font contains a Unicode cmap */

    if(pthread_mutex_init(&lock, NULL) != 0)
    {
        debugNetPrintf(ERROR,"\n mutex init failed\n");
    }

    // return NOW, follows an example
    return;

// from example1
  char*         text = "hella";
  int           n, num_chars;
  num_chars     = strlen( text );

  int target_height = ATTR_HEIGHT;  

  FT_GlyphSlot  slot;
  slot = face->glyph;

  FT_Vector     pen;                    /* untransformed origin  */
  FT_Matrix     matrix;                 /* transformation matrix */
  double        angle = ( 25.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cosf( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sinf( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sinf( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cosf( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (100,200) relative to the upper left corner  */
  pen.x = 100 * 64;
  pen.y = ( target_height - 200 ) * 64;

    for ( n = 0; n < num_chars; n++ )
      {
        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
        if ( error )
          continue;                 /* ignore errors */

        /* now, draw to our target surface (convert position) */
        /*draw_bitmap(&slot->bitmap,
                      slot->bitmap_left,
                      target_height - slot->bitmap_top );*/

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
      }
}


/* cleanup freetype */
void FT_end(void)
{
    FT_Error error;

    error = FT_Done_Face( face );
    debugNetPrintf(DEBUG,"FT_Done_Face return:\t%d\n", error);

    error = FT_Done_FreeType( library );
    debugNetPrintf(DEBUG,"FT_Done_FreeType return:\t%d\n", error);

    pthread_mutex_destroy(&lock);

    // release data buffer (readed from host0)
    if(buffer) { free(buffer), buffer = NULL; }
}