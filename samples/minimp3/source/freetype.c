/*
 * freetype example1.c
 * liborbis port, 2018
 * masterzorag@gmail.com
 */

#include "freetype.h"
extern Orbis2dConfig *orbconf;  // hook main 2d conf to get framebuffer address
static uint32_t __attribute__((aligned(16))) *pixel = NULL;


static FT_Library library;
static FT_Face    face = NULL;
static FT_Byte *buffer = NULL;  // stores ttf font data


/* precomputed gradient colors [0-7] */
#define SIMPLE_GRADIENT_STEPS  (8)
static uint32_t fading_color[SIMPLE_GRADIENT_STEPS];

static uint32_t c1 = 0xFFFFFFFF,
                c2 = 0xFF404040;

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


/* internally draw via liborbis2d */
static void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y) // glyph refs
{
    uint32_t pixelColor = 0xFF000000;
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
                pixelColor = ARGB(0xFF, c, c, c);
                /*pixelColor=(c + 1)<<24  //R
                          |(c + 1)<<16  //G
                          |(c + 1)<<8   //B
                          |0xFF;*/

                orbis2dDrawPixelColor(i, j, pixelColor);
            } else if(c > 0x80) {
                //pixelColor = ARGB(0xFF, 0x10 + c + q*2, 0x10 + c + q*2, 0x10 + c + q*2);
                /*pixelColor=(c - q*1)<<24  //R
                          |(c - q*5)<<16  //G
                          |(c - q*1)<<8   //B
                          |0xFF;*/
                //orbis2dDrawPixelColor(i, j, pixelColor);
                orbis2dDrawPixelColor(i, j, fading_color[q /3]);
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
typedef int v2i __attribute__((ext_vector_type(2)));

// we save (rendered) last text length in pixel, to align center/right
static v2i baseline;

static v2i FT_loop_text(uint dst_x, uint dst_y, const char *text, int render_or_not)
{
    if(!face) return 0;

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

        if(render_or_not)
        {
            /* now, draw to our target surface (convert position) */
            draw_bitmap(&slot->bitmap,
                         slot->bitmap_left,
                         ATTR_HEIGHT - slot->bitmap_top);
        }
        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
        //debugNetPrintf(DEBUG,"[%d] %d %d %d %d\n", n, slot->bitmap_left, slot->bitmap_top, pen.x /64, pen.y /64);
    }

    /* save text length in pixel */
    if(!render_or_not) {
        baseline.x = (pen.x - dst_x * 64) /64;
        baseline.y = (pen.y - (ATTR_HEIGHT - dst_y) * 64) /64;
        //debugNetPrintf(DEBUG,"baseline, x: %d\ty: %d\n", baseline.x, baseline.y);
    } else {
        /* now we can also underline text! */
        /* disabled!
        orbis2dDrawLineColor(dst_x, dst_y, dst_x + baseline.x, dst_y - baseline.y, 0xFFFF0000);  // red
        */
    }

    return baseline;
}

/* facilities */

int FT_get_text_lenght(uint dst_x, uint dst_y, const char *text)
{
    v2i res = FT_loop_text(dst_x, dst_y, text, 0);  // pass 1: compute lenght
    //printf("res: %d, %d\n", res.x, res.y);
    return res.x;
}

/* print from main() */
void FT_print_text(uint dst_x, uint dst_y, const char *text)
{
    FT_loop_text(dst_x, dst_y, text, 1);  // pass 2: render text
}

void FT_set_text_angle(double new_angle)
{
    angle = new_angle;
}

/* set character size */
void FT_set_text_size(int pt, int dpi)
{
    FT_Error error = FT_Set_Char_Size( face, pt * 64, 0, dpi, 0 );
    /* error handling omitted */
    if(error)
        debugNetPrintf(DEBUG,"FT_Set_Char_Size return:\t%d\n", error);
}

/* initialize library,
   create face object,
   set character size */
void FT_init(void)
{
    FT_update_gradient(&c1, &c2);

    FT_Error error;

    /* initialize library */
    error = FT_Init_FreeType( &library );
    /* error handling omitted */
    debugNetPrintf(DEBUG,"FT_Init_FreeType return:\t%d, library at %p\n", error, library);

    /* load default font */
    if(!buffer) buffer = orbisFileGetFileContent("host0:zrnic_rg.ttf");

    if(!buffer) return;
    // passed, report
    debugNetPrintf(DEBUG,"data buffer at %p, size %u\n", buffer, _orbisFile_lastopenFile_size);

    /* create face object */
    if(!face)
    {
        error = FT_New_Memory_Face( library,
                                    buffer,                         /* first byte in memory */
                                    _orbisFile_lastopenFile_size,   /* size in bytes        */
                                    0,                              /* face_index           */
                                    &face );
        /* error handling omitted */
        debugNetPrintf(DEBUG,"FT_New_Memory_Face return:\t%d, face at:\t%p\n", error, face);
    }

    /* use 16pt at 100dpi */
    FT_set_text_size(16, 100);

    FT_set_text_angle(0.0);

    /* cmap selection omitted;                                        */
    /* for simplicity we assume that the font contains a Unicode cmap */

    sleep(1);

    return;
}


/* cleanup freetype */
void FT_end(void)
{
    FT_Error error;

    error = FT_Done_Face( face );
    debugNetPrintf(DEBUG,"FT_Done_Face return:\t%d\n", error);

    error = FT_Done_FreeType( library );
    debugNetPrintf(DEBUG,"FT_Done_FreeType return:\t%d\n", error);

    // release data buffer (readed from HOST0:)
    if(buffer) { free(buffer), buffer = NULL; }
}
