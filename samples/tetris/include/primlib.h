#ifndef __PRIMLIB_H__
#define __PRIMLIB_H__


#include <orbis2d.h>
#include <orbisXbmFont.h>
#include <debugnet.h>


enum color {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX};
void pixel(int x, int y, enum color c);
void line(int x1, int y1, int x2, int y2, enum color c);
void circle(int x, int y, int r, enum color c);
void filledRect(int x1, int y1, int x2, int y2, enum color c);
void filledCircle(int x, int y, int r, enum color c);
void rect(int x1, int y1, int x2, int y2, enum color c);
void textout(int x, int y, char *s, enum color c);

void updateScreen();
int  pollkey();
int  getkey();
int  isKeyDown(int key);

#endif /* __PRIMLIB_H__ */
