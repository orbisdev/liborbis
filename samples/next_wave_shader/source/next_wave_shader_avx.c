/*
    AVX implementation of GLSL shader from http://glslsandbox.com/e#57140

    #liborbis port, 2019, masterzorag@gmail.com

    - http://software-lisc.fbk.eu/avx_mathfun/
    - https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __PS4__
#include <orbis2d.h>  // ATTR_*
extern Orbis2dConfig *orbconf;  // hook main 2d conf to get framebuffer address
#else
#include "pixel.h"         // SDL bindings: hook the SDL_surface from main() with init_pixel_SDL()
#include "orbisXbmFont.h"  // ARGB
#endif

static uint32_t *pixel = NULL;


// Clang Extended Vectors
typedef float vec2 __attribute__((ext_vector_type(2)));
typedef float vec3 __attribute__((ext_vector_type(3)));
typedef float vec4 __attribute__((ext_vector_type(4)));


static float Time = 0.f;
/*uniform vec2 mouse; */

//static const vec2 resolution = { ATTR_WIDTH, ATTR_HEIGHT };
//static       vec4 gl_FragColor;
static const vec2 __attribute__((aligned(16))) step = { 1. /ATTR_WIDTH, 1. /ATTR_HEIGHT };

#include <immintrin.h>    // oh, yes!
#include "avx_mathfun.h"  // sin256_ps()


#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif


/* http://glslsandbox.com/e#57140 */

#if 0  /* original shader */
#endif


/* AVX implementation */
void glsl_e57140_avx( void )
{ 
    //Time = 1.f;
    register uint32_t *pixel = (uint32_t *)orbconf->surfaceAddr[orbconf->currentBuffer];
    
    register vec2 gl_FragCoord = { 0., 1. };  // draw from upperleft
    
    register short w, h;
  //  Uint32 color = 0xFFFF0000; //ARGB red
//vec3 col[4]; // m128i
    __m256 __attribute__((aligned(32))) px, py,     // position
                                        t1, t2,     // tmp
                                        _r, _g, _b, // out x8 rgb
                           /*_sPos_x,*/ _sPos_y;

    for(h=0; h < ATTR_HEIGHT; h++)
    {
        //const float py1 = gl_FragCoord.y;   
        py = _mm256_set1_ps( -1.0 + 2.0 * gl_FragCoord.y );  //	position = -1.0 + 2.0 * position; x8 Y
        for(w=0; w < ATTR_WIDTH /8; w++)
        {
          /* vec2 position = ( gl_FragCoord.xy / resolution.xy ); */
            t1 = _mm256_mul_ps( _mm256_set_ps( 7., 6., 5., 4., 3., 2., 1., 0. ),
                                _mm256_set1_ps( step.x ) );       // t1 = incremental step.x 
            t2 = _mm256_add_ps( _mm256_set1_ps( gl_FragCoord.x ),
                                 t1 );                            // t2 = px (x8)

            /*vec3 final_color = vec3(0.);*/
            _r = _mm256_set1_ps( 0. ), _g = _mm256_set1_ps( 0. ), _b = _mm256_set1_ps( 0. );

          /* creating the wave */
          // position = -1.0 + 2.0 * position; x8 X
        	px = _mm256_mul_ps( t2, _mm256_set1_ps(  2. ) );	px = _mm256_add_ps( _mm256_set1_ps( -1. ), px );
        	// vec2 sPos = position ;
        	_sPos_y = py;

          for( float i = 0.0; i < 8.; i++)
          {
          /* sPos.y += (0.07 * sin( position.x + i/5.0 + time*2. )); */
              t1 = _mm256_set1_ps( i/5.0 + Time*1. );
              t1 = _mm256_add_ps ( px, t1 );
              t2 =  sin256_ps(t1);
              t1 = _mm256_mul_ps( t2, _mm256_set1_ps( 0.07 ) );
         _sPos_y = _mm256_add_ps( _sPos_y, t1 );

           /* lineWidth   = abs(1.0 / (1000.0 * sPos.y)); */
              t2 = _mm256_mul_ps( _mm256_set1_ps( 1000.0 ), _sPos_y );
              t2 = _mm256_div_ps( _mm256_set1_ps(    1.5 ), t2 );
              t2 = _mm256_max_ps(_mm256_sub_ps(_mm256_setzero_ps(), t2), t2); // fabs() (x4f)

           /* final_color = final_color
                          + vec3(lineWidth * 2.9,     R
                                 lineWidth*0.,        G
                                 lineWidth * 1.);     B
           */
              t1 = _mm256_mul_ps( _mm256_set1_ps( 2.9 ), t2 );  _r = _mm256_add_ps( _r, t1 );
              t1 = _mm256_mul_ps( _mm256_set1_ps( 0.  ), t2 );  _g = _mm256_add_ps( _g, t1 );
              t1 = _mm256_mul_ps( _mm256_set1_ps( 1.  ), t2 );  _b = _mm256_add_ps( _b, t1 );
          }
          /* sanitize vectors for RGB, this is not GLSL! */
          /* clamp 0.f-1.f */
            _r = _mm256_min_ps( _mm256_max_ps(_r, _mm256_set1_ps( 0. )), _mm256_set1_ps( 1.f ) ); // x8 R
            _r = _mm256_mul_ps( _r, _mm256_set1_ps( 0xFF ));
            _g = _mm256_min_ps( _mm256_max_ps(_g, _mm256_set1_ps( 0. )), _mm256_set1_ps( 1.f ) ); // x8 G
            _g = _mm256_mul_ps( _g, _mm256_set1_ps( 0xFF ));
            _b = _mm256_min_ps( _mm256_max_ps(_b, _mm256_set1_ps( 0. )), _mm256_set1_ps( 1.f ) ); // x8 B
            _b = _mm256_mul_ps( _b, _mm256_set1_ps( 0xFF ));
            
            /* unpack, compose pixel (x8) colors */
            #pragma clang loop unroll_count(8)
            for(int i=0; i<8; i++)
            {
               /* paint pixel */
               *pixel++ = ARGB(0xFF, (unsigned char)(_r[i]),
                                     (unsigned char)(_g[i]),
                                     (unsigned char)(_b[i]));
            }
            gl_FragCoord.x += step.x *8; // advance X
        }
        gl_FragCoord.x  = 0.,
        gl_FragCoord.y -= step.y; // advance Y
    
        Time += 0.0001976; // animate
    }
    //Time += 0.01976 *2 ; // animate
}
