#ifndef _MENU_H_
#define _MENU_H_

#include <debugnet.h>

#define BCKGR_FILE_PATH "host0:img/background.png"

#define TERM_FILE_PATH "host0:img/terminal/terminal.png"
#define TERM_TOP_FILE_PATH "host0:img/terminal/terminal_top.png"
#define TERM_BOT_FILE_PATH "host0:img/terminal/terminal_bot.png"

#define ICON1S_FILE_PATH "host0:img/115px/games_115.png"
#define ICON2S_FILE_PATH "host0:img/115px/files_115.png"
#define ICON3S_FILE_PATH "host0:img/115px/tools_115.png"
#define ICON4S_FILE_PATH "host0:img/115px/network_115.png"
#define ICON5S_FILE_PATH "host0:img/115px/settings_115.png"

#define ICON1_FILE_PATH "host0:img/135px/games_135.png"
#define ICON2_FILE_PATH "host0:img/135px/files_135.png"
#define ICON3_FILE_PATH "host0:img/135px/tools_135.png"
#define ICON4_FILE_PATH "host0:img/135px/network_135.png"
#define ICON5_FILE_PATH "host0:img/135px/settings_135.png"

//menuconf
int menu_slide;
int select_theme;
int x1;
int64_t flipArg;

enum ICON_LEVEL {
	ICON_1,
	ICON_2,
	ICON_3,
	ICON_4,
	ICON_5,
};

enum SubMenuLevel {
	Theme_Select,
	Slide_Menu,
};

int iconInit();
int iconGetLevel();
void iconSetLevel(int levelicon);
void iconLevelLeft();
void iconLevelRight();


int SubMenuGetLevel();
void SubMenuSetLevel(int levelsubmenu);
void SubMenuLevelDown();
void SubMenuLevelUp();

#endif
