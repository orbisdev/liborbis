#include <stdio.h>
#include <stdlib.h>

#include <orbisPad.h>
#include <orbis2d.h>
#include <orbisXbmFont.h>
#include "menu.h"


// General background
extern Orbis2dTexture *bckgr;
Orbis2dTexture *bckgr=NULL;

// Terminal windows Images (for submenu or text)
extern Orbis2dTexture *term;
extern Orbis2dTexture *termtop;
extern Orbis2dTexture *termbot;
Orbis2dTexture *term=NULL;
Orbis2dTexture *termtop=NULL;
Orbis2dTexture *termbot=NULL;

// Top menu icons Small
extern Orbis2dTexture *icon1s;
extern Orbis2dTexture *icon2s;
extern Orbis2dTexture *icon3s;
extern Orbis2dTexture *icon4s;
extern Orbis2dTexture *icon5s;
Orbis2dTexture *icon1s=NULL;
Orbis2dTexture *icon2s=NULL;
Orbis2dTexture *icon3s=NULL;
Orbis2dTexture *icon4s=NULL;
Orbis2dTexture *icon5s=NULL;

// Top menu icons Big
extern Orbis2dTexture *icon1;
extern Orbis2dTexture *icon2;
extern Orbis2dTexture *icon3;
extern Orbis2dTexture *icon4;
extern Orbis2dTexture *icon5;
Orbis2dTexture *icon1=NULL;
Orbis2dTexture *icon2=NULL;
Orbis2dTexture *icon3=NULL;
Orbis2dTexture *icon4=NULL;
Orbis2dTexture *icon5=NULL;

// Define icons size status
int icon1big=0;
int icon2big=0;
int icon3big=1;
int icon4big=0;
int icon5big=0;

// Top Menu Tittles
#define tittle_1 "----G A M E S----"
#define tittle_2 "----F I L E S----"
#define tittle_3 "----T O O L S----"
#define tittle_4 "--N E T W O R K--"
#define tittle_5 "-S E T T I N G S-"
char *tittle=tittle_3;
int32_t tittle_y=210;
char tmp_ln[256];


//Top Menu icons
int iconLevel=ICON_3;
x1=0;
int y1=20;
int step1=140;


// terminal
int termx;
int termy = 200;
int iterm;
int tx;


int tabx=200;
int taby=200;
int tabw=880;
int tabh=400;
int rect=1;
int slidetab=1;
uint32_t color=0xFF000000;
uint32_t c1=0xFF000000;
uint32_t c2=0xFF000000;

//Sub_Menu Setting menu conf temp
#define slide_on 	"MENU SLIDE = ON  - Press X to switch"
#define slide_off	"MENU SLIDE = OFF - Press X to switch"
char *theme_text=	"THEME BY DEFAULT - WOODY_GEEK";
char *slide_text=slide_off;

int sub_text_y=250;
int sub_text_step=25;

int submenulevel=Theme_Select;
menu_slide=0;
select_theme=0;


int hig_x=340;
int hig_w=600;
int hig_y=250;
int hig_h=18;
int hig_step=25;
uint32_t hig_color=0x800080ff;



void MenuDraw()
{
	if(menu_slide==0)
	{
		x1=0;
	}
	if(bckgr)
	{
		orbis2dDrawTexture(bckgr,0,0);		
	}

	if(icon1 && icon1big==1)
	{			
		orbis2dDrawTexture(icon1,x1+292,y1);		
	}
	else if (icon1s && icon1big==0)
	{
		orbis2dDrawTexture(icon1s,x1+302,y1);	
	}

	if(icon2 && icon2big==1)
	{
		orbis2dDrawTexture(icon2,x1+432,y1);
	}
	else if (icon2s && icon2big==0)
	{
		orbis2dDrawTexture(icon2s,x1+442,y1);	
	}

	if(icon3 && icon3big==1)
	{
		orbis2dDrawTexture(icon3,x1+572,y1);
	}
	else if (icon3s && icon3big ==0)
	{
		orbis2dDrawTexture(icon3s,x1+582,y1);
	}
		
	if(icon4 && icon4big==1)
	{
		orbis2dDrawTexture(icon4,x1+712,y1);
	}
	else if (icon4s && icon4big ==0)
	{
		orbis2dDrawTexture(icon4s,x1+722,y1);	
	}

	if(icon5 && icon5big==1)
	{
		orbis2dDrawTexture(icon5,x1+852,y1);
	}
	else if (icon5s && icon5big ==0)
	{
		orbis2dDrawTexture(icon5s,x1+862,y1);	
	}
		
}

void SubMenuDraw()
{
	if(rect == 1 && slidetab==0)
	{
		//for (int iterm=0;iterm<tabh;iterm++) // need to split in 3 images then slide display
		//{	
			
			//debugNetPrintf(DEBUG,"Valeur de iterm %d \n",iterm);
			termx = get_image_aligned_x(term, CENTER);
			orbis2dDrawTexture(termbot,termx,termy+iterm);
			orbis2dDrawTexture(termtop,termx,termy);
			if (iterm>=34) orbis2dDrawRectColor(termx, tabw, termy+34 , iterm-33, color);

			c1=0x00000000;
			c2=0xFF000000;
			update_gradient(&c1, &c2);
			sprintf(tmp_ln, tittle);
			tx = get_aligned_x(tmp_ln, CENTER);
			print_text(tx, tittle_y, tittle);
			//orbis2dFinishDrawing(flipArg);

				//}
			//if (iterm==tabh) slidetab=1;
			if (iterm!=tabh) iterm++;
	
	}
	else if (rect == 1 && slidetab==1)
	{
		termx = get_image_aligned_x(termtop, CENTER);
		orbis2dDrawTexture(termtop,termx,termy);

		c1=0xFF000000;
		c2=0xFF000000;
		update_gradient(&c1, &c2);
		sprintf(tmp_ln, tittle);
		tx = get_aligned_x(tmp_ln, CENTER);
		print_text(tx, tittle_y, tittle);
	}

	if(icon5big==1 && iterm==tabh)
	{
		orbis2dDrawRectColor(hig_x, hig_w, hig_y, hig_h, hig_color);

		c1=0xFF4ce600;
		c2=0xFF4ce600;
		update_gradient(&c1, &c2);

		sprintf(tmp_ln, theme_text);
		tx = get_aligned_x(tmp_ln, CENTER);
		print_text(tx, sub_text_y, theme_text);
			
		int ty=sub_text_y + sub_text_step;
		sprintf(tmp_ln, slide_text);
		tx = get_aligned_x(tmp_ln, CENTER);
		print_text(tx, ty, slide_text);
	}
}


int BackgroundInit()
{
	int ret=0;
	bckgr=orbis2dLoadPngFromHost_v2(BCKGR_FILE_PATH);
	if(bckgr)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",BCKGR_FILE_PATH);
		
	}
	return ret;
}

int TermInit()
{
	int ret=0;
	term=orbis2dLoadPngFromHost_v2(TERM_FILE_PATH);
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

	icon1s=orbis2dLoadPngFromHost_v2(ICON1S_FILE_PATH);
	if(icon1s)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON1S_FILE_PATH);
		
	}

	icon1=orbis2dLoadPngFromHost_v2(ICON1_FILE_PATH);
	if(icon1)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON1_FILE_PATH);
		
	}

	icon2s=orbis2dLoadPngFromHost_v2(ICON2S_FILE_PATH);
	if(icon2s)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON2S_FILE_PATH);
		
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

	icon3s=orbis2dLoadPngFromHost_v2(ICON3S_FILE_PATH);
	if(icon3s)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON3S_FILE_PATH);
		
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

	icon4s=orbis2dLoadPngFromHost_v2(ICON4S_FILE_PATH);
	if(icon4s)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON4S_FILE_PATH);
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
	
	icon5s=orbis2dLoadPngFromHost_v2(ICON5S_FILE_PATH);
	if(icon5s)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON5S_FILE_PATH);
	}

	icon5=orbis2dLoadPngFromHost_v2(ICON5_FILE_PATH);
	if(icon5)
	{
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading png image file from %s \n",ICON5_FILE_PATH);
	}
	return ret;
 
}



int iconGetLevel()
{
	return iconLevel;
}
void iconSetLevel(int levelicon)
{
	if(levelicon>=0)
	{
		iconLevel=levelicon;
	}	
}
void iconLevelLeft()
{
	switch(iconGetLevel())
	{
		case ICON_1:
			break;
		case ICON_2:
			slidetab=1;
			iconSetLevel(ICON_1);
			icon1big=1;
			icon2big=0;
			x1=x1+step1;
			rect=1;
			tabh=300;
			tittle=tittle_1;
			break;
		case ICON_3:
			slidetab=1;
			iconSetLevel(ICON_2);
			icon2big=1;
			icon3big=0;
			x1=x1+step1;
			rect=1;
			tabh=400;
			tittle=tittle_2;
			break;
		case ICON_4:
			slidetab=1;
			iconSetLevel(ICON_3);
			icon3big=1;
			icon4big=0;
			x1=x1+step1;
			rect=1;
			tabh=200;
			tittle=tittle_3;
			break;
		case ICON_5:
			slidetab=1;
			iconSetLevel(ICON_4);
			icon4big=1;
			icon5big=0;
			x1=x1+step1;
			rect=1;
			tabh=300;
			tittle=tittle_4;
			break;
		default:
			break;
	}
}

void iconLevelRight()
{
	switch(iconGetLevel())
	{
		case ICON_1:
			slidetab=1;
			iconSetLevel(ICON_2);
			icon2big=1;
			icon1big=0;
			x1=x1-step1;
			rect=1;
			tabh=400;
			tittle=tittle_2;
			break;
		case ICON_2:
			slidetab=1;
			iconSetLevel(ICON_3);
			icon3big=1;
			icon2big=0;
			x1=x1-step1;
			rect=1;
			tabh=200;
			tittle=tittle_3;
			break;
		case ICON_3:
			slidetab=1;
			iconSetLevel(ICON_4);
			icon4big=1;
			icon3big=0;
			x1=x1-step1;
			rect=1;
			tabh=300;
			tittle=tittle_4;
			break;
		case ICON_4:
			iterm=0;
			slidetab=0;
			iconSetLevel(ICON_5);
			icon5big=1;
			icon4big=0;
			x1=x1-step1;
			rect=1;
			tabh=80;
			tittle=tittle_5;
			break;
		case ICON_5:
			break;
		default:
			break;
	}
}

int SubMenuGetLevel()
{
	return submenulevel;
}
void SubMenuSetLevel(int levelsubmenu)
{
	if(levelsubmenu>=0)
	{
		submenulevel=levelsubmenu;
	}	
}

void SubLevelDown()
{
	if(iconGetLevel()==ICON_5)
	{
		switch(SubMenuGetLevel())
		{
		case Theme_Select:
			SubMenuSetLevel(Slide_Menu);
			hig_y=hig_y+hig_step;
			break;
		case Slide_Menu:
			break;
		default:
			break;
		}
	}
}

void SubLevelUp()
{
	if(iconGetLevel()==ICON_5)
	{
		switch(SubMenuGetLevel())
		{
		case Theme_Select:
			break;
		case Slide_Menu:
			SubMenuSetLevel(Theme_Select);
			hig_y=hig_y-hig_step;
			break;
		default:
			break;
		}
	}
}

void SetValue()
{
	if(iconGetLevel()==ICON_5 && SubMenuGetLevel()==Slide_Menu)
	{
		if(menu_slide==0)
		{
		slide_text=slide_on;
		menu_slide=1;
		x1=-280;
		}
		else
		{
		slide_text=slide_off;
		menu_slide=0;
		}
	}

}
