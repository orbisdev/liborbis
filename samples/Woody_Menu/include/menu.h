#ifndef _MENU_H_
#define _MENU_H_


#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <debugnet.h>



#define TERM_FILE_PATH "host0:img/terminal/terminal.png"
#define TERM_TOP_FILE_PATH "host0:img/terminal/terminal_top.png"
#define TERM_BOT_FILE_PATH "host0:img/terminal/terminal_bot.png"

#define iconGamesSmall_FILE_PATH "host0:img/115px/games_115.png"
#define icon2Small_FILE_PATH "host0:img/115px/files_115.png"
#define icon3Small_FILE_PATH "host0:img/115px/tools_115.png"
#define icon4Small_FILE_PATH "host0:img/115px/network_115.png"
#define iconSettingsSmall_FILE_PATH "host0:img/115px/settings_115.png"

#define iconGames_FILE_PATH "host0:img/135px/games_135.png"
#define ICON2_FILE_PATH "host0:img/135px/files_135.png"
#define ICON3_FILE_PATH "host0:img/135px/tools_135.png"
#define ICON4_FILE_PATH "host0:img/135px/network_135.png"
#define iconSettings_FILE_PATH "host0:img/135px/settings_135.png"


// Top Menu Tittles
#define tittle_Games "----G A M E S----"
#define tittle_2 "----F I L E S----"
#define tittle_3 "----T O O L S----"
#define tittle_4 "--N E T W O R K--"
#define tittle_Settings "-S E T T I N G S-"
int32_t tittle_y;
char tmp_ln[256];


//menuconf
int menu;
int menu_slide;
int select_theme;
int main_menu_x;
int64_t flipArg;


//submenu
int tx;
int ty;

// terminal
int terminal;
int termx;
int slidespeed;
int termy;
int iterm;

int temp_flipArg;



int tabx;
int taby;
int tabw;
int tabh;
int slidetab;
int color;
int c1;
int c2;


// sub menu text
int sub_text_y;
int sub_text_step;
int sub_hig_x;
int sub_hig_w;
int sub_hig_y;
int sub_hig_h;
int sub_hig_step;
int sub_hig_color;

enum ICON_LEVEL {
	Icon_Games,
	ICON_2,
	ICON_3,
	ICON_4,
	Icon_Settings,
};


int iconInit();
int iconGetLevel();
void iconSetLevel(int levelicon);
void mainMenuLevelLeft();
void mainMenuLevelRight();


int SubMenuSettingsGetLevel();
void SubMenuSettingsetLevel(int levelSubMenuSetting);
void SubMenuSettingLevelDown();
void SubMenuSettingLevelUp();

#endif
