#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>


#include "primlib.h"

static uint32_t colors[COLOR_MAX] =
{ 0xff000000, 0xffff0000, 0xff00ff00, 0xff0000ff,
  0xff00ffff, 0xffffff00, 0xffff00ff, 0xffffffff };

void pixel(int x, int y, enum color c)
{
    orbis2dDrawPixelColor(x,y,colors[c]);
}

void line(int x1, int y1, int x2, int y2, enum color c)
{
    orbis2dDrawLineColor(x1,y1,x2,y2,colors[c]);
}

void rect(int x1, int y1, int x2, int y2, enum color c)
{
    //orbis2dDrawBoxColor(int x, int w, int y, int h, uint32_t color)
    orbis2dDrawBoxColor(x1,x2-x1,y1,y2-y1, colors[c]);
}
//void orbis2dDrawRectColor(int x, int w, int y, int h, uint32_t color)
void filledRect(int x1, int y1, int x2, int y2, enum color c)
{
    orbis2dDrawRectColor(x1,x2-x1,y1,y2-y1,colors[c]);
}

void circle(int x, int y, int r, enum color c)
{
    orbis2dDrawCircleColor(x,y,r,0,colors[c]);
}

void filledCircle(int x, int y, int r, enum color c)
{
    orbis2dDrawCircleColor(x,y,r,1,colors[c]);
}

void updateScreen()
{
    //SDL_Flip(screen);
}

void textout(int x, int y, char *s, enum color c)
{
    print_text(x, y, s);
}

int pollkey()
{
    /*SDL_Event event;
    while ( SDL_PollEvent(&event) ) 
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                return event.key.keysym.sym;
            case SDL_QUIT:
                exit(3);
        }
    }*/
    return -1;
}

int getkey()
{
    /*SDL_Event event;
    while(1)
    {
      SDL_WaitEvent(&event);
      if(event.type==SDL_KEYDOWN)
          break;
      if(event.type==SDL_QUIT)
          exit(3);
    };
    return event.key.keysym.sym;*/
    return 0;
}

int isKeyDown(int key)
{
 /*uint8* keytable;
 int numkeys;
 SDL_PumpEvents();
 keytable=SDL_GetKeyState(&numkeys);
 assert(key<numkeys);
 return keytable[key];*/
 return 0;
}
