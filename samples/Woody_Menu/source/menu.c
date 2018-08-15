#include <stdio.h>
#include <stdlib.h>

#include <orbisPad.h>
#include <orbis2d.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "settings.h"


//active menu
menu=1;

// Terminal windows Images (for SubMenuSetting or text)
extern Orbis2dTexture *term;
extern Orbis2dTexture *termtop;
extern Orbis2dTexture *termbot;
Orbis2dTexture *term=NULL;
Orbis2dTexture *termtop=NULL;
Orbis2dTexture *termbot=NULL;

// Top menu icons Small
extern Orbis2dTexture *iconGamesSmall;
extern Orbis2dTexture *icon2Small;
extern Orbis2dTexture *icon3Small;
extern Orbis2dTexture *icon4Small;
extern Orbis2dTexture *iconSettingsSmall;
Orbis2dTexture *iconGamesSmall=NULL;
Orbis2dTexture *icon2Small=NULL;
Orbis2dTexture *icon3Small=NULL;
Orbis2dTexture *icon4Small=NULL;
Orbis2dTexture *iconSettingsSmall=NULL;

// Top menu icons Big
extern Orbis2dTexture *iconGames;
extern Orbis2dTexture *icon2;
extern Orbis2dTexture *icon3;
extern Orbis2dTexture *icon4;
extern Orbis2dTexture *iconSettings;
Orbis2dTexture *iconGames=NULL;
Orbis2dTexture *icon2=NULL;
Orbis2dTexture *icon3=NULL;
Orbis2dTexture *icon4=NULL;
Orbis2dTexture *iconSettings=NULL;

// Define icons size status
int iconGamesBig=0;
int icon2big=0;
int icon3big=1;
int icon4big=0;
int iconSettingsBig=0;

// Top Menu Tittles
char *tittle=tittle_3;
tittle_y=210;


//Main Menu icons
char mainMenuLevel=ICON_3;
main_menu_x=0;
int main_menu_y=20;
int main_menu_step_x=140;


//terminal
terminal=1;
slidespeed=8;
termy = 200;
temp_flipArg=0;

tabx=200;
taby=200;
tabw=880;
tabh=0;
color=0xFF000000;

//submenu text / higlight
int setsub()
{
	sub_text_y=250;
	sub_text_step=25;
	sub_hig_x=340;
	sub_hig_w=600;
	sub_hig_y=250;
	sub_hig_h=18;
	sub_hig_step=25;
	sub_hig_color=0x800080ff;
}

void MainMenuDraw()
{
	if(menu_slide==0)
	{
		main_menu_x=0;
	}

	if(iconGames && iconGamesBig==1)
	{			
		orbis2dDrawTexture(iconGames,main_menu_x+292,main_menu_y);		
	}
	else if (iconGamesSmall && iconGamesBig==0)
	{
		orbis2dDrawTexture(iconGamesSmall,main_menu_x+302,main_menu_y);	
	}

	if(icon2 && icon2big==1)
	{
		orbis2dDrawTexture(icon2,main_menu_x+432,main_menu_y);
	}
	else if (icon2Small && icon2big==0)
	{
		orbis2dDrawTexture(icon2Small,main_menu_x+442,main_menu_y);	
	}

	if(icon3 && icon3big==1)
	{
		orbis2dDrawTexture(icon3,main_menu_x+572,main_menu_y);
	}
	else if (icon3Small && icon3big ==0)
	{
		orbis2dDrawTexture(icon3Small,main_menu_x+582,main_menu_y);
	}
		
	if(icon4 && icon4big==1)
	{
		orbis2dDrawTexture(icon4,main_menu_x+712,main_menu_y);
	}
	else if (icon4Small && icon4big ==0)
	{
		orbis2dDrawTexture(icon4Small,main_menu_x+722,main_menu_y);	
	}

	if(iconSettings && iconSettingsBig==1)
	{
		orbis2dDrawTexture(iconSettings,main_menu_x+852,main_menu_y);
	}
	else if (iconSettingsSmall && iconSettingsBig ==0)
	{
		orbis2dDrawTexture(iconSettingsSmall,main_menu_x+862,main_menu_y);	
	}
		
}

void TermTopDraw()
{
	{

		//DRAW TOP OF WINDOWS
		orbis2dDrawTexture(termtop,termx,termy);

		//print tittle over the top windows image
		c1=0xFF000000;
		c2=0xFF000000;
		update_gradient(&c1, &c2);
		sprintf(tmp_ln, tittle);
		tx = get_aligned_x(tmp_ln, CENTER);
		print_text(tx, tittle_y, tittle);
	}
}



void TerminalDraw()
{
	if(slide_term_status==1)
	{


		//DRAW BOTTOM OF WINDOWS (Will down during the count)
		orbis2dDrawTexture(termbot,termx,termy+iterm);

		//DRAW TOP OF WINDOWS	
		if (iterm<=34)
		{
			orbis2dDrawTexture(termtop,termx,termy);

			//print tittle over the top windows image
			c1=0xFF000000;
			c2=0xFF000000;
			update_gradient(&c1, &c2);
			sprintf(tmp_ln, tittle);
			tx = get_aligned_x(tmp_ln, CENTER);
			print_text(tx, tittle_y, tittle);
		}

		//complet in black between top and bottom
		if (iterm>=34) orbis2dDrawRectColor(termx, tabw, termy+34 , iterm-33, color);

		
		// need to count frame buffer then iterm++ (TODO: find a way to use a timer)
		if (iterm+slidespeed<tabh)
		{
			if (flipArg!=temp_flipArg && flipArg!=temp_flipArg+1)
			{
				for(int i=0;i<=slidespeed;i++)iterm++;
				temp_flipArg=flipArg;
			}
			else if (flipArg==temp_flipArg+1)
			{
				for(int i=0;i<slidespeed;i++)iterm++;
				temp_flipArg=flipArg;
			}

		}
		// need to count frame buffer then iterm-- (TODO: find a way to use a timer)
		else if (iterm-slidespeed>tabh)
		{
			if (flipArg!=temp_flipArg && flipArg!=temp_flipArg+1)
			{
				for(int i=0;i<=slidespeed;i++)iterm--;
				temp_flipArg=flipArg;
			}
			else if (flipArg==temp_flipArg+1)
			{
				for(int i=0;i<slidespeed;i++)iterm--;
				temp_flipArg=flipArg;
			}
		}
		else iterm=tabh;

	}
	else if (slide_term_status==0)
	{
		iterm=tabh;
		//complet in black between top and bottom
		orbis2dDrawRectColor(termx, tabw, termy+34 , iterm-33, color);

		//DRAW BOTTOM OF WINDOWS (Will down during the count)
		orbis2dDrawTexture(termbot,termx,termy+iterm);

		//DRAW TOP OF WINDOWS	
		orbis2dDrawTexture(termtop,termx,termy);

		// print tittle menu
		c1=0xFF000000;
		c2=0xFF000000;
		update_gradient(&c1, &c2);
		sprintf(tmp_ln, tittle);
		tx = get_aligned_x(tmp_ln, CENTER);
		print_text(tx, tittle_y, tittle);
	}

	// draw submenu when terminal is complet drawed
	if(iconSettingsBig==1 && iterm==tabh) SubMenuSettingsDraw();
	if(iconGamesBig==1 && iterm==tabh) SubMenuGamesDraw();

}


int TermInit()
{
	int ret=0;
	term=orbis2dLoadPngFromHost_v2(TERM_FILE_PATH);
	termx = orbis2dGetTextureXAlign(term, CENTER);
	if(term)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",TERM_FILE_PATH);
		
	}
	termtop=orbis2dLoadPngFromHost_v2(TERM_TOP_FILE_PATH);
	if(termtop)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",TERM_TOP_FILE_PATH);
		
	}
	termbot=orbis2dLoadPngFromHost_v2(TERM_BOT_FILE_PATH);
	if(termbot)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",TERM_BOT_FILE_PATH);
		
	}
	return ret;
}

int IconsInit()
{
	int ret=0;

	iconGamesSmall=orbis2dLoadPngFromHost_v2(iconGamesSmall_FILE_PATH);
	if(iconGamesSmall)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",iconGamesSmall_FILE_PATH);
		
	}

	iconGames=orbis2dLoadPngFromHost_v2(iconGames_FILE_PATH);
	if(iconGames)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",iconGames_FILE_PATH);
		
	}

	icon2Small=orbis2dLoadPngFromHost_v2(icon2Small_FILE_PATH);
	if(icon2Small)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",icon2Small_FILE_PATH);
		
	}
	icon2=orbis2dLoadPngFromHost_v2(ICON2_FILE_PATH);
	if(icon2)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON2_FILE_PATH);
		
	}

	icon3Small=orbis2dLoadPngFromHost_v2(icon3Small_FILE_PATH);
	if(icon3Small)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",icon3Small_FILE_PATH);
		
	}

	icon3=orbis2dLoadPngFromHost_v2(ICON3_FILE_PATH);
	if(icon3)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON3_FILE_PATH);
	}

	icon4Small=orbis2dLoadPngFromHost_v2(icon4Small_FILE_PATH);
	if(icon4Small)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",icon4Small_FILE_PATH);
	}

	icon4=orbis2dLoadPngFromHost_v2(ICON4_FILE_PATH);
	if(icon4)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON4_FILE_PATH);
	}
	
	iconSettingsSmall=orbis2dLoadPngFromHost_v2(iconSettingsSmall_FILE_PATH);
	if(iconSettingsSmall)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",iconSettingsSmall_FILE_PATH);
	}

	iconSettings=orbis2dLoadPngFromHost_v2(iconSettings_FILE_PATH);
	if(iconSettings)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",iconSettings_FILE_PATH);
	}
	return ret;
 
}



int iconGetLevel()
{
	return mainMenuLevel;
}
void iconSetLevel(int levelicon)
{
	if(levelicon>=0)
	{
		mainMenuLevel=levelicon;
	}	
}
void mainMenuLevelLeft()
{
	switch(iconGetLevel())
	{
		case Icon_Games:
			break;
		case ICON_2:
			iconSetLevel(Icon_Games);
			iconGamesBig=1;
			icon2big=0;
			main_menu_x=main_menu_x+main_menu_step_x;
			tabh=100;
			tittle=tittle_Games;
			break;
		case ICON_3:
			iconSetLevel(ICON_2);
			icon2big=1;
			icon3big=0;
			main_menu_x=main_menu_x+main_menu_step_x;
			tabh=0;
			tittle=tittle_2;
			break;
		case ICON_4:
			iconSetLevel(ICON_3);
			icon3big=1;
			icon4big=0;
			main_menu_x=main_menu_x+main_menu_step_x;
			tabh=0;
			tittle=tittle_3;
			break;
		case Icon_Settings:
			iconSetLevel(ICON_4);
			icon4big=1;
			iconSettingsBig=0;
			main_menu_x=main_menu_x+main_menu_step_x;
			tabh=0;
			tittle=tittle_4;
			break;
		default:
			break;
	}
}

void mainMenuLevelRight()
{
	switch(iconGetLevel())
	{
		case Icon_Games:
			iconSetLevel(ICON_2);
			icon2big=1;
			iconGamesBig=0;
			main_menu_x=main_menu_x-main_menu_step_x;
			tabh=0;
			tittle=tittle_2;
			break;
		case ICON_2:
			iconSetLevel(ICON_3);
			icon3big=1;
			icon2big=0;
			main_menu_x=main_menu_x-main_menu_step_x;
			tabh=0;
			tittle=tittle_3;
			break;
		case ICON_3:
			iconSetLevel(ICON_4);
			icon4big=1;
			icon3big=0;
			main_menu_x=main_menu_x-main_menu_step_x;
			tabh=0;
			tittle=tittle_4;
			break;
		case ICON_4:
			iconSetLevel(Icon_Settings);
			SubMenuSettingsetLevel(Theme_Select);
			iconSettingsBig=1;
			icon4big=0;
			main_menu_x=main_menu_x-main_menu_step_x;
			tabh=250;
			tittle=tittle_Settings;
			break;
		case Icon_Settings:
			break;
		default:
			break;
	}
}