/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl, Byron Ellacott - bje@apnic.net

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

/* 07-14-02 Major speed optimizations done by WP (it's really fast now :-)). Uses an 8 bit indexed image*/
/* note that the code has not been fully optimized */
/* 07-21-18 liborbis port uses 32bpp image and tile, but we don't use liborbis2dDrawPixel(),
 * so we need to pass current drawing framebuffer pointer, for each frame drawn;
 * masterzorag@gmail.com
 */


#include <stdio.h>
#include <stdlib.h>   // random(), RAND_MAX
#include <math.h>     // sinf()
#include <orbis2d.h>  // orbis2dDrawPixelColor()
#include <debugnet.h>

#include "rotozoom.h"
#define PNG_FILE_PATH  "host0:mz.png"

static int roto[256];
static int roto2[256];
static unsigned int path = 0, zpath = 0;

static Orbis2dTexture *tile = NULL;
static uint32_t *texture    = NULL;
static uint32_t *image      = NULL;
static uint32_t tw          = 0;     // tile width


static void draw_tile(int stepx, int stepy, int zoom)
{
    if(!texture) return;

    register int x, y, xd, yd, a, b, sx, sy;

    sx = sy = 0;
    xd = (stepx * zoom) >> 12;
    yd = (stepy * zoom) >> 12;

    /* Stepping across and down the screen, each screen row has a
     * starting coordinate in the texture: (sx, sy).    As each screen
     * row is traversed, the current texture coordinate (x, y) is
     * modified by (xd, yd), which are (sin(rot), cos(rot)) multiplied
     * by the current zoom factor.    For each vertical step, (xd, yd)
     * is rotated 90 degrees, to become (-yd, xd).
     *
     * More fun can be had by playing around with x, y, xd, and yd as
     * you move about the image.
     */

    for(int j = 0; j < ATTR_HEIGHT; j++)
    {
        x = sx; y = sy;
        for(int i = 0; i < ATTR_WIDTH; i++)
        {
            a = x >> 12 & 255;
            b = y >> 12 & 255;

            int idx = b * tw + a;
            if(idx < 0 || idx >= 256*256) debugNetPrintf(DEBUG, "%d, ", idx);

            *image++ = texture[idx];
            x += xd; y += yd;
        }
        sx -= yd; sy += xd;
    }
    image = NULL;  // keep safe, don't leave pointer there
}


void init_rotozoom(void)
{
    for(int i = 0; i < 256; i++)
    {
        float rd = (float)i * 1.41176 * 0.0174532;
        float c  = sinf(rd);
        roto[i]  = (c + 0.8) * 4096.0;
        roto2[i] = (2.0 * c) * 4096.0;
    }

    // load the background image
    if(!tile) tile=orbis2dLoadPngFromHost_v2(PNG_FILE_PATH);

    if(!tile)
        debugNetPrintf(DEBUG,"Problem loading tile image file from %s \n",PNG_FILE_PATH);
    else
    {
        debugNetPrintf(DEBUG, "init return tile at 0x%p\n", tile);
        debugNetPrintf(DEBUG, "tile width:%d height:%d depth:%d\n", tile->width, tile->height, tile->depth);

        if(tile->width != 256 || tile->height != 256)
        {
            debugNetPrintf(DEBUG, "Tile dimensions must be 256x256!\n");

            // we can use tile in other ways, or release and free
            orbis2dDestroyTexture(tile);
            return;
        }

        // passed, setup
        texture = orbis2dTextureGetDataPointer(tile);    // address texture, once
        tw = tile->width;
    }
}


void draw_rotozoom(const uint32_t *dst)
{
    image = (uint32_t *)dst;  // address current framebuffer, each frame

    /* draw the tile at current zoom and rotation */
    draw_tile(roto[path], roto[(path + 128) & 255], roto2[zpath]);

    path  = (path  - 1) & 255;
    zpath = (zpath + 1) & 255;
}


void destroy_rotozoom(void)
{
    if(tile) orbis2dDestroyTexture(tile);
}
