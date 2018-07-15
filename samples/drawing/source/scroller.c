/*
  testing sinf() from math.h
  masterzorag@gmail.com, 2015-2018
*/


#include <math.h>          // sinf()
#include <string.h>        // strlen()
#include <orbis2d.h>       // ATTR_WIDTH
#include <orbisXbmFont.h>  // print_text()


#define STEP_X  -2         // horizontal displacement

static const char s[] = "since alpha-blending is too slow, we pass over for now, but anything can't stop us, we can rule the world, muaahahaha";

static int sx = ATTR_WIDTH,
           sl = 0;


/***********************************************************************
* Compute string len once, then reuse value
***********************************************************************/
void init_sinetext(void)
{
    sl = strlen(s) * FONT_W;
}


/***********************************************************************
* Draw a string of chars, amplifing y by sin(x)
***********************************************************************/
void draw_sinetext(const int y)
{
    unsigned int i;
    int x = sx;       // every call resets the initial x
    float amp;
    char c[2];

    c[1] = '\0';      // print_text() deals with '\0' terminated

    for(i = 0; i < (sl / FONT_W); i++)
    {
        amp = sinf(x      // testing asinf() from math.h
                 * 0.02)  // it turns out in num of bends
                 * 20;    // +/- vertical bounds over y

        c[0] = s[i];      // split text into single characters

        if(x > 0
        && x < ATTR_WIDTH - FONT_W)
            print_text(x, y + amp, c);

        x += FONT_W;
    }
}


/***********************************************************************
* Move string by defined step
***********************************************************************/
void move_sinetext(void)
{
    sx += STEP_X;

    if(sx < -sl)           // horizontal bound, then loop
        sx = ATTR_WIDTH + FONT_W;
}
