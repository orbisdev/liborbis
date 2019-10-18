/*
    AVX implementation of GLSL shader, base from http://glslsandbox.com/e#56480.0 and modified a little

    #liborbis port, 2019, masterzorag@gmail.com

    - http://gruntthepeon.free.fr/ssemath/
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

static const vec2 __attribute__((aligned(16))) step = { 1. /ATTR_WIDTH, 1. /ATTR_HEIGHT };

#include <immintrin.h>    // oh, yes!
#include "avx_mathfun.h"  // sin256_ps()


#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif


/* http://glslsandbox.com/e#56480.0 */

#if 0  /* original shader */
#endif


/* AVX implementation */
void glsl_e56480_avx( void )
{
    //Time = 1.f;
    register uint32_t *pixel        = (uint32_t *)orbconf->surfaceAddr[orbconf->currentBuffer];
    register vec2      gl_FragCoord = { 0., 1. };  // draw from upperleft
    register short     w, h;

    __m256 __attribute__((aligned(32))) px, py,  // position
                                        t1, t2,  // tmp
                                        _b,      // out x8 blue
                                        _s;      // x8 sinus
    const float scaleImage = 10.0;
    //_s = _mm256_set1_ps( 0. );  // reset at new frame

    for(h=0; h < ATTR_HEIGHT; h++)
    {
        //_s = _mm256_set1_ps( 0. );          // reset at new frame
        //_s = _mm256_set1_ps( h * step.y );  // reset at new h

        py = _mm256_set1_ps( gl_FragCoord.y /** scaleImage*/ );  // x8 Y
        py = _mm256_mul_ps ( py, _mm256_set1_ps( scaleImage ) );
        for(w=0; w < ATTR_WIDTH /8; w++)
        {
         /* vec2 position = ( gl_FragCoord.xy / resolution.xy ); */
            t1 = _mm256_mul_ps( _mm256_set_ps( 7., 6., 5., 4., 3., 2., 1., 0. ),
                                _mm256_set1_ps( step.x ) );        // t1 = incremental step.x 
            px = _mm256_add_ps( _mm256_set1_ps( gl_FragCoord.x ),
                                t1 );                              // px (x8)

            /* vec4 color = vec4(0.0); */
            _b = _mm256_set1_ps( 0. );

            px = _mm256_mul_ps ( px, _mm256_set1_ps( scaleImage ) );
          //_s = _mm256_set1_ps( (1.+w)/(2.+h) / scaleImage );  // reset sinus
            _s = _mm256_set1_ps( 0./*w * step.x*/  );  // reset sinus

            for (float j = 0.; j < 2.; j++)  // accumulate sinus
            {
                /*sinus += sin( (0.+j)
                     * pos.x + time*2.)
                     + pos.y - scaleImage/3.0;*/

                 t1 = _mm256_add_ps( px, _mm256_set1_ps( Time * 0.15 ) );  // t1 = pos.x + time
                 t2 = _mm256_add_ps( _mm256_set1_ps( 0. ), _mm256_set1_ps( j ) );
                 t1 = _mm256_mul_ps( t1, t2 );
                 t2 =  sin256_ps   ( t1 );

                 t2 = _mm256_add_ps( t2, py );
                 t2 = _mm256_sub_ps( t2, _mm256_set1_ps( scaleImage/2.5 ) );
                 _s = _mm256_add_ps( _s, t2 );
            }
            t1 = _mm256_mul_ps( _mm256_set1_ps( 1.1 ), _s );
            _b = _mm256_add_ps( _b, _mm256_set1_ps( 1.0 ) );
            _b = _mm256_sub_ps( _b, t1 );

            /* sanitize vectors for RGB, this is not GLSL!: clamp 0.f-1.f */
            _b = _mm256_min_ps( _mm256_max_ps(_b, _mm256_set1_ps( 0. )), _mm256_set1_ps( 1.f ) );  // x8 B
            _b = _mm256_mul_ps( _b, _mm256_set1_ps( 0xFF ));

            /* unpack, compose pixel (x8) colors */
            #pragma clang loop unroll_count(8)
            for(int i=0; i<8; i++)
            {
               /* paint pixel */
               *pixel++ = ARGB(0xFF, (unsigned char)( 0.2 ),
                                     (unsigned char)( 0.1 ),
                                     (unsigned char)(_b[i] * gl_FragCoord.y));
            }
            gl_FragCoord.x += step.x *8; // advance X
        }
        gl_FragCoord.x  = 0.,
        gl_FragCoord.y -= step.y; // advance Y

        Time += 0.0001976; // animate
    }
}