#include <stdio.h>
#include <stdlib.h>

#include <orbis2d.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "games.h"

//Sub_Menu games Conf

	//snake
	char *snake_text="PLAY SNAKE (v1 Beta)";
	snake=0;

void SubMenuGamesDraw()
{
	
	orbis2dDrawRectColor(sub_hig_x, sub_hig_w, sub_hig_y, sub_hig_h, sub_hig_color);

	c1=0xFF4ce600;
	c2=0xFF4ce600;
	update_gradient(&c1, &c2);

	sprintf(tmp_ln, snake_text);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(tx, sub_text_y, snake_text);
}

void SetGamesValue()
{
	// lauch snake
	if(iconGetLevel()==Icon_Games)
	{
		snake=1;
		menu=0;
		terminal=0;

	}
}