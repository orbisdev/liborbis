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

#include <stdio.h>
#include <stdlib.h>   // random(), RAND_MAX
#include <orbis2d.h>  // orbis2dDrawPixelColor()

#include "starfield.h"

static STAR stars[NUMBER_OF_STARS];

void init_star(STAR *star, const uint16_t i)
{
    /* randomly init stars, generate them around the center of the screen */
    star->xpos =  -10.0 + (20.0 * (random()/(RAND_MAX+1.0)));
    star->ypos =  -10.0 + (20.0 * (random()/(RAND_MAX+1.0)));

    /* change viewpoint */
    star->xpos *= 3072.0;
    star->ypos *= 3072.0;
    star->zpos =  i;
    star->speed = 1 + (int)(2.0 * (random()/(RAND_MAX+1.0)));

    star->color = i >> 2; /*the closer to the viewer the brighter*/
}

void init_starfield(/* stars, first run */)
{
  uint16_t i;
  for(i = 0; i < NUMBER_OF_STARS; i++)
    init_star(stars +i, i +1);
}

void draw_starfield(/* we don't pass the framebuffer */)
{
    int tx, ty;
    uint16_t i;

    for(i = 0; i < NUMBER_OF_STARS; i++)
    {
        stars[i].zpos -= stars[i].speed;

        if(stars[i].zpos <= 0) init_star(stars +i, i +1);

        /* compute 3D position */
        tx = (stars[i].xpos / stars[i].zpos) + (ATTR_WIDTH  >>1);
        ty = (stars[i].ypos / stars[i].zpos) + (ATTR_HEIGHT >>1);

        /* check if a star leaves the screen */
        if(tx < 0 || tx > ATTR_WIDTH -1
        || ty < 0 || ty > ATTR_HEIGHT -1)
        {
            //sys_log("star[%d]: @%.4d, %.4d exited the screen, re-init\n", i, tx, ty);
            init_star(stars +i, i);
            continue;
        }

        /* draw to screen via liborbis2d */
        orbis2dDrawPixelColor(tx, ty, stars[i].color<<24
                                    | stars[i].color<<16
                                    | stars[i].color<<8
                                    | stars[i].color);
    }
}
