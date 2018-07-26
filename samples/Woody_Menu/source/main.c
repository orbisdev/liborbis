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

void updateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");

		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed exit\n");
			flag=0;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed\n");
			SetValue();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			//debugNetPrintf(DEBUG,"L1 pressed\n");
			//slidetab=0;
			iconLevelLeft();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			//debugNetPrintf(DEBUG,"R1 pressed\n");
			//slidetab=0;
			iconLevelRight();

		}
		if(orbisPadGetButtonPressed(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");
			SubLevelUp();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			SubLevelDown();
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

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer(1);
				
		//draw menu
		MenuDraw();

		//draw sub_menu
		SubMenuDraw();
				
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
