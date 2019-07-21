/*
  A Raster Twister Demo, ps4 liborbis port

  2017, masterzorag@gmail.com

  main function taken from https://github.com/Acedio/demo-effects/blob/master/twist.c

  code rewritten to use GCC/Clang vector extensions:

  https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html
  https://clang.llvm.org/docs/LanguageExtensions.html#vector-literals

  we ca also pass -ffast-math to compiler
*/
#include <stdio.h>
#include <stdlib.h>         // random(), RAND_MAX
#include <orbis2d.h>        // orbis2dDrawPixelColor(), ATTR_WIDTH, ATTR_HEIGHT
#include <orbisXbmFont.h>   // ARGB facilities
#include <math.h>           // sin(), cos()


#include "twist.h"


#define PI                3.14159
#define WAVECOUNT         3
#define TWISTER_W        (ATTR_WIDTH /6)
#define VERTICAL_BORDER   0


static uint32_t face_A = 0xFFAA11CC;  // default colors
static uint32_t face_B = 0xFF881155;

static int frames = 0;                           // keep track of frames
static float amps[WAVECOUNT], freqs[WAVECOUNT];  // precomputed values


void update_twist_color(uint32_t *a, uint32_t *b)
{
    face_A = *a;
    face_B = *b;
}


void Twister_init(void)
{
    srand(time(0));

    for(int i = 0; i < WAVECOUNT; i++)
    {
        amps[i] = (float)((random()>>4)%50)/10.0;
        freqs[i] = 4.00/(float)(1+((random()>>4)%50));
        printf("amp: %f   freq: %f\n",amps[i],freqs[i]);
    }
}


/* main function start here */
static void twist(float *amps, float *freqs, int wavecount, int t)
{
    float theta, _sin, _cos;  // to reuse them

    uint16_t x, y, stop;
    float4   c4, w4;          // first splats fraction, c4 vector multiply each color component 
    uint32_t color;

    for(y = 0 + VERTICAL_BORDER; y < ATTR_HEIGHT - VERTICAL_BORDER; y++)  // draw y lines from the top of the screen
    {
        /* compute theta */
        theta = 0;
        for(int i = 0; i < wavecount; i++){
            theta += amps[i] * sin((float)(y+t) * freqs[i]/1000);
        }
        /* force theta to 0-PI */
        while(theta > PI){ theta -= PI; }
        while(theta < 0) { theta += PI; }

        _sin = sin(theta);
        _cos = cos(theta);

        if(theta < PI/2)  /* we enter this loop OR the reversed below */
        {
            w4 = TWISTER_W * (float4){ (_sin + _cos),  // used in x, fraction
                                        0,             // unused
                                        _sin,          // used in stop 1, FACE_A
                                        _cos };        // used in stop 2, FACE_B 
            x = stop = (ATTR_WIDTH - w4.x) /2 -100;    // reset twister pos, centered -100
            stop += w4.z;                              // accumulate
            c4    = ( (float) w4.z/w4.x )              // splats fraction into vector
                  * (float4){ GET_A(face_A), GET_R(face_A), GET_G(face_A), GET_B(face_A) };  // apply fraction to target [ARGB]

            color = ARGB ((uint8_t)c4.x, (uint8_t)c4.y, (uint8_t)c4.z, (uint8_t)c4.w);  // extract and convert
         // color = ((uint8_t)c4.x << 16) | ((uint8_t)c4.y << 8) | ((uint8_t)c4.z);
            for(; x < stop; x++) {
                orbis2dDrawPixelColor(x, y, color);  /* draw to screen via liborbis2d */
            }
            stop += w4.w;                            // accumulate
            c4    = ( (float) w4.w/w4.x )            // splats fraction into vector
                  * (float4){ GET_A(face_B), GET_R(face_B), GET_G(face_B), GET_B(face_B) };
            color = ARGB ((uint8_t)c4.x, (uint8_t)c4.y, (uint8_t)c4.z, (uint8_t)c4.w);  // color = (r << 16) | (g << 8)| (b);
            for(; x < stop; x++) {
                orbis2dDrawPixelColor(x, y, color);  /* draw to screen via liborbis2d */
            }
        }
        else /* if(theta > PI/2), like the one before, but in reverse order ! */
        {
            w4 = TWISTER_W * (float4){ 0,             // unused
                                      (_sin - _cos),  // used in x, fraction
                                       _sin,          // used in stop 2, FACE_A
                                       -_cos };       // used in stop 1, FACE_B
            x = stop = (ATTR_WIDTH - w4.y) /2 -100;   // reset twister pos, centered -100
            stop += w4.w;
            c4    = ( (float) w4.w/w4.y )             // splats fraction into vector
                  * (float4){ GET_A(face_B), GET_R(face_B), GET_G(face_B), GET_B(face_B) };
            color = ARGB ((uint8_t)c4.x, (uint8_t)c4.y, (uint8_t)c4.z, (uint8_t)c4.w);
            for(; x < stop; x++) {
                orbis2dDrawPixelColor(x, y, color);  /* draw to screen via liborbis2d */
            }
            stop += w4.z;
            c4    = ( (float) w4.z/w4.y )             // splats fraction into vector
                  * (float4){ GET_A(face_A), GET_R(face_A), GET_G(face_A), GET_B(face_A) };  // a fraction of base color
            color = ARGB ((uint8_t)c4.x, (uint8_t)c4.y, (uint8_t)c4.z, (uint8_t)c4.w);
            for(; x < stop; x++){
                orbis2dDrawPixelColor(x, y, color);  /* draw to screen via liborbis2d */
            }
        }
    } // end draw for y
}


void Twister_draw(void) // will also count frames !
{
    twist(amps, freqs, WAVECOUNT, frames * 10);  // speed is related to frames

    frames++;
}

