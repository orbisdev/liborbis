// L1 Move Top Menu left
// L2 Move top menu Right
// X Active / Desactive / Access
// /\ to exit



#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <orbis2d.h>
#include <orbisPad.h>
#include <orbisKeyboard.h>
#include <orbisAudio.h>
#include <modplayer.h>
#include <ps4link.h>
#include <debugnet.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "snake.h"
#include "settings.h"

//main conf
	static char refresh = 1;
	int flag=0;

	Orbis2dConfig *conf;
	OrbisPadConfig *confPad;

	typedef struct OrbisGlobalConf
	{
		Orbis2dConfig *conf;
		OrbisPadConfig *confPad;
		OrbisAudioConfig *confAudio;
		OrbisKeyboardConfig *confKeyboard;
		ps4LinkConfiguration *confLink;
		int orbisLinkFlag;
	}

	OrbisGlobalConf;
	OrbisGlobalConf *myConf;


// General background
	#define BCKGR_FILE_PATH "host0:img/background.png"
	extern Orbis2dTexture *bckgr;
	Orbis2dTexture *bckgr=NULL;

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

	void BackGoundDraw()
	{
		if(bckgr)
		{
			orbis2dDrawTexture(bckgr,0,0);		
		}
	}




void updateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		//main
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			flag=0;
		}

		//menu
		if (menu==1)
		{
			if(orbisPadGetButtonPressed(ORBISPAD_L1))
			{
				if (slide_term_switch==0) iterm=0;
				setsub();
				mainMenuLevelLeft();
				refresh = 1;
			}
			if(orbisPadGetButtonPressed(ORBISPAD_R1))
			{
				if (slide_term_switch==0) iterm=0;
				setsub();
				mainMenuLevelRight();
				refresh = 1;
			}
		}

		//games sub menu
		if(iconGetLevel()==Icon_Games && terminal==1)
		{
			if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
			{
				SetGamesValue();
				refresh = 1;
			}
		}

		//settings sub menu
		if(iconGetLevel()==Icon_Settings && terminal==1)
		{
			if(orbisPadGetButtonPressed(ORBISPAD_UP))
			{
				debugNetPrintf(DEBUG,"Up pressed\n");
				SubMenuSettingsLevelUp();
			}
			if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
			{
				debugNetPrintf(DEBUG,"Down pressed\n");
				SubMenuSettingsLevelDown();
			}
			if(orbisPadGetButtonPressed(ORBISPAD_RIGHT))
			{
				debugNetPrintf(DEBUG,"Right pressed\n");
				if(SubMenuSettingsGetLevel()==Slide_Term_Speed) slidespeed++;
			}
			if(orbisPadGetButtonPressed(ORBISPAD_LEFT))
			{
				debugNetPrintf(DEBUG,"Left pressed\n");
				if (SubMenuSettingsGetLevel()==Slide_Term_Speed && slidespeed>1) slidespeed--;
			}
			if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
			{
				debugNetPrintf(DEBUG,"Cross pressed\n");
				SetSettingsValue();
				refresh=1;
			}
		}

		//snake
		if (snake==1)
		{
			if(orbisPadGetButtonPressed(ORBISPAD_UP) && speedys != 1)
			{
				debugNetPrintf(DEBUG,"Snake Up pressed\n");
				updateSnakeDir(0,-1);
			}
			if(orbisPadGetButtonPressed(ORBISPAD_DOWN) && speedys!=-1)
			{
				debugNetPrintf(DEBUG,"Down pressed\n");
				updateSnakeDir(0,1);
			}
			if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) && speedxs!=-1)
			{
				debugNetPrintf(DEBUG,"Right pressed\n");
				updateSnakeDir(1,0);
			}
			if(orbisPadGetButtonPressed(ORBISPAD_LEFT) && speedxs!=1)
			{
				debugNetPrintf(DEBUG,"Left pressed\n");
				updateSnakeDir(-1,0);
			}
			if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
			{
				debugNetPrintf(DEBUG,"Circle pressed come back to MSX\n");
				snake=0;
				menu=1;
				terminal=1;
				refresh=1;
			}
		}	
	}
}


void initApp()
{
	int ret;

	debugNetPrintf(DEBUG,"[PS4LINK] Initialized and connected from pc/mac ready to receive commands\n");
	
	//hide playroom splash
	sceSystemServiceHideSplashScreen();

	ret=orbisPadInitWithConf(myConf->confPad);
	
	if(ret==1)
	{
		
	    confPad=orbisPadGetConf();
		ret=orbis2dInitWithConf(myConf->conf);
			
		if(ret==1)
		{
			conf=orbis2dGetConf();
			flag=1;
		}
	}
	
}

void finishApp()
{
	orbisAudioFinish();
	orbisPadFinish();
	
	orbis2dFinish();
	
	ps4LinkFinish();	
}

int main(int argc, char *argv[])
{
	int ret;


	uintptr_t intptr=0;
	sscanf(argv[1],"%p",&intptr);
	myConf=(OrbisGlobalConf *)intptr;
	ret=ps4LinkInitWithConf(myConf->confLink);
	if(!ret)
	{
		ps4LinkFinish();
		return 0;
	}
	initApp();
   	BackgroundInit();
   	TermInit();
   	IconsInit();

	
	while(flag)
	{
		//capture pad data and populate positions
		updateController();
						
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear the current display buffer
		orbis2dClearBuffer(0);

		if(refresh)	// draw the background image
		{
			orbis2dClearBuffer(1);  // don't use dumpBuf, force clean

			// first, an image
			BackGoundDraw();
			//draw menu
			if(menu==1)
			{
				MainMenuDraw();
				TermTopDraw();
			}
			orbis2dDumpBuffer(), refresh = 0;  // save dumpBuf
			debugNetPrintf(DEBUG,"orbis2dDumpBuffer()\n");
		}

		//draw sub_menu_games
		//if(terminal==1 && iconGetLevel()==Icon_Games)SubMenuGamesDraw();

		//draw sub_menu_settings
		//if(terminal==1 && iconGetLevel()==Icon_Settings)SubMenuSettingsDraw();
		if (terminal==1) TerminalDraw();
		//draw snake
		if (snake==1) drawSnake();

		//flush and flip
		orbis2dFinishDrawing(flipArg);
				
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
	}
	
	//wait for current display buffer
	orbis2dStartDrawing();

	// clear with background (default white) to the current display buffer 
	orbis2dClearBuffer(1);
					
	//flush and flip
	orbis2dFinishDrawing(flipArg);
			
	//swap buffers
	orbis2dSwapBuffers();
	
	finishApp();
	
	myConf->orbisLinkFlag=1;

	return 0;
}
