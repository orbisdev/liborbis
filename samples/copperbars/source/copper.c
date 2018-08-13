/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl
   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.
   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* note that the code has not been fully optimized */
/* 07-21-18 liborbis port uses liborbis2dDrawPixel, no need to address current framebuffer;
 * most of initialization has been rewritten to use loops intead of fixed definitions;
 * masterzorag@gmail.com
 */


#include <stdio.h>
#include <stdlib.h>        // random(), RAND_MAX
#include <math.h>          // sinf()
#include <orbis2d.h>       // orbis2dDrawPixelColor()
#include <orbisXbmFont.h>  // ARGB()

#include "copper.h"

static uint32_t color[46];
static uint16_t aSin[360];


static void compose_fadebar(uint32_t *color, uint8_t idx)
{
    /* compose 15 faded colors, (up to 0xFF, stepping 0x20) for each of 15 lines */
    uint8_t step = 0x20, c = step - 1;

    uint32_t *p = &color[idx];    // address pointer to indexed color

    for(uint8_t i=0; i<15; i++)
    {
        switch(idx)
        {
            /* red copper bar */
            case 1:  *p = ARGB(0xFF, c, 0, 0); break;

            /* white copper bar */
            case 16: *p = ARGB(0xFF, c, c, c); break;

            /* blue copper bar */
            case 31: *p = ARGB(0xFF, 0, 0, c); break;

            default: break;
        }
        if(i == 7) step *= -1;  // change step direction

        c += step, p++;
    }
    p = NULL;  // keep safe, don't leave pointer there
}


void init_copperbars(void)
{
    int i, centery = ATTR_HEIGHT >> 1;
    float rad;

    /* create sin lookup table */
    for (i = 0; i < 360; i++)
    {
        rad =    (float)i * 0.0174532; 
        aSin[i] = centery + (sinf(rad) * 100.0);
    }

    /* precompute colors */
    compose_fadebar(&color[0], 1);     /* red   copper bar */
    compose_fadebar(&color[0], 16);    /* white copper bar */
    compose_fadebar(&color[0], 31);    /* blue  copper bar */
}


static void draw_copper(uint16_t * const y, int add)
{
    for(int i = 0; i < 15; i++)  // bar height, 15px
    {
        /* use precomputed colors */
        orbis2dDrawLineColor(0, *y, ATTR_WIDTH, *y, color[i + add]);
        *y += 1;
    }
}


static uint16_t
    red    = 96, red2   = 0, red3   = 88, red4   = 0,
    red5   = 80, red6   = 0, red7   = 72, red8   = 0,
    white  = 64, white2 = 0, white3 = 56, white4 = 0,
    white5 = 48, white6 = 0, white7 = 40, white8 = 0,
    blue   = 32, blue2  = 0, blue3  = 24, blue4  = 0,
    blue5  = 16, blue6  = 0, blue7  =  8, blue8  = 0;


void draw_copperbars(void)
{
    /* draw copperbars back to front */
    uint16_t y;

    y = aSin[blue7];  blue8 = y;  blue7 += 2;  blue7 %= 360;
    draw_copper(&y, 31);

    y = aSin[blue5];  blue6 = y;  blue5 += 2;  blue5 %= 360;
    draw_copper(&y, 31);

    y = aSin[blue3];  blue4 = y;  blue3 += 2;  blue3 %= 360;
    draw_copper(&y, 31);

    y = aSin[blue];   blue2 = y;  blue += 2;   blue %= 360;
    draw_copper(&y, 31);

    y = aSin[white7]; white8 = y; white7 += 2; white7 %= 360;
    draw_copper(&y, 16);

    y = aSin[white5]; white6 = y; white5 += 2; white5 %= 360;
    draw_copper(&y, 16);

    y = aSin[white3]; white4 = y; white3 += 2; white3 %= 360;
    draw_copper(&y, 16);

    y = aSin[white];  white2 = y; white += 2;  white %= 360;
    draw_copper(&y, 16);

    y = aSin[red7];   red8 = y;   red7 += 2;   red7 %= 360;
    draw_copper(&y, 1);

    y = aSin[red5];   red6 = y;   red5 += 2;   red5 %= 360;
    draw_copper(&y, 1);

    y = aSin[red3];   red4 = y;   red3 += 2;   red3 %= 360;
    draw_copper(&y, 1);

    y = aSin[red];    red2 = y;   red += 2;    red %= 360;
    draw_copper(&y, 1);
}
