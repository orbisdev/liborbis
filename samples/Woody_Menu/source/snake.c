#include <stdio.h>
#include <stdlib.h>

#include <orbis2d.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "games.h"
#include "snake.h"


int scanvasx = 390;
int scanvasy = 10;
int scanvasw = 500;
int scanvash = 640;

int playzonex = 440;
int playzoney = 220;
int playzonew = 400;
int playzoneh = 400;

char *snake_tittle="SNAKE (v1 Beta)";
int snake_tittle_y=20;

uint32_t colorSnake=0xFF4ce600;
xs=441;
ys=221;
speedxs=10;
speedys=0;

void updateSnakeDir(int x, int y)
{
	speedxs=x*10;
	speedys=y*10;
}


void drawSnake()
{
	orbis2dDrawRectColor(scanvasx, scanvasw, scanvasy , scanvash, color);

	orbis2dDrawRectColor(playzonex, playzonew, playzoney , playzoneh, 0x9099ccff);
	

	//print tittle over the top windows image
	c1=0xFF4ce600;
	c2=0xFF4ce600;
	update_gradient(&c1, &c2);
	sprintf(tmp_ln, snake_tittle);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(tx, snake_tittle_y, snake_tittle);

	orbis2dDrawRectColor(xs,10,ys,10,colorSnake);
	if (xs+speedxs>playzonex && xs+speedxs<playzonex+playzonew) xs=xs+speedxs;
	if (ys+speedys>playzoney && ys+speedys<playzoney+playzoneh) ys=ys+speedys;
}