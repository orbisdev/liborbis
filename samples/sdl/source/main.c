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
#include <SDL2/SDL_config.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>


int x=1280/2;
int y=720/2;
int w=1280/64;
int h=1280/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
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
}OrbisGlobalConf;

OrbisGlobalConf *myConf;

//SDL stuff for dummies
SDL_Rect fillRect;
SDL_Window    * gWindow   = NULL;
SDL_Renderer  * gRenderer = NULL;
// Variables for controllers and joysticks
SDL_GameController *controller = NULL;
SDL_Joystick *joy = NULL;

void updateControllerSDL()
{
	SDL_Event e;
	unsigned int buttons=0;
	
	SDL_JoystickUpdate();
	while(SDL_PollEvent(&e)!=0)
	{
		//SDL_Log("SDL %d %d %d\n",e.type,SDL_CONTROLLERBUTTONDOWN,SDL_CONTROLLERBUTTONUP);
		
		if(e.type == SDL_QUIT)
			SDL_Log("SDL quit\n");
            
		else
		{
			switch(e.type)
			{
				case SDL_JOYAXISMOTION: 
							// handle axis motion
					break;
				
				case SDL_JOYBUTTONDOWN:
					//SDL_Log("SDL button down\n");
					if(SDL_JoystickGetButton(joy,12) && SDL_JoystickGetButton(joy,13))
					{
						SDL_Log("Combo L2R2 pressed\n");
						buttons=orbisPadGetCurrentButtonsPressed();
						buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
						orbisPadSetCurrentButtonsPressed(buttons);
					}
					if(SDL_JoystickGetButton(joy,8) && SDL_JoystickGetButton(joy,9))
					{
						SDL_Log("Combo L1R1 pressed\n");
						buttons=orbisPadGetCurrentButtonsPressed();
						buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
						orbisPadSetCurrentButtonsPressed(buttons);
			
					}
					if(SDL_JoystickGetButton(joy,8) && SDL_JoystickGetButton(joy,13))
					{
						SDL_Log("Combo L1R2 pressed\n");
						buttons=orbisPadGetCurrentButtonsPressed();
						buttons&= ~(ORBISPAD_L1|ORBISPAD_R2);
						orbisPadSetCurrentButtonsPressed(buttons);
			
			
					}
					if(SDL_JoystickGetButton(joy,12) && SDL_JoystickGetButton(joy,9))
					{
						SDL_Log("Combo L2R1 pressed\n");
						buttons=orbisPadGetCurrentButtonsPressed();
						buttons&= ~(ORBISPAD_L2|ORBISPAD_R1);
						orbisPadSetCurrentButtonsPressed(buttons);
			
					}
					if(SDL_JoystickGetButton(joy,4))
					{
						SDL_Log("Up pressed\n");
		
						if(y-step>=0)
						{
							y=y-step;
						}
						else
						{
							y=0;
						}
					}
					if(SDL_JoystickGetButton(joy,6))
					{
						SDL_Log("Down pressed\n");
		
						if(y+step<conf->height-1)
						{
							y=y+step;
						}
						else
						{
							y=conf->height-1-step;
						}
					}						
					if(SDL_JoystickGetButton(joy,5))
					{
						SDL_Log("Right pressed\n");
		
						if(x+step<conf->width-1)
						{
							x=x+step;
						}
						else
						{
							x=conf->width-1-step;
						}
					}
					if(SDL_JoystickGetButton(joy,7))
					{
						SDL_Log("Left pressed\n");
		
						if(x-step>=0)
						{
							x=x-step;
						}
						else
						{
							x=0;
						}
					}
					if(SDL_JoystickGetButton(joy,0))
					{
						SDL_Log("Triangle pressed exit\n");
			
						flag=0;
				
					}
					if(SDL_JoystickGetButton(joy,1))
					{
						SDL_Log("Circle pressed reset position and color red\n");
						x=1280/2;
						y=720/2;
						color=0x80ff0000;	
						orbisAudioResume(0);
			
					}
					if(SDL_JoystickGetButton(joy,2))
					{
						SDL_Log("Cross pressed rand color\n");
						R=rand()%256;
						G=rand()%256;
						B=rand()%256;
						color=0x80000000|R<<16|G<<8|B;
						orbisAudioStop();
			
					}
					if(SDL_JoystickGetButton(joy,3))
					{
						SDL_Log("Square pressed\n");
						orbisAudioPause(0);
			
					}
					if(SDL_JoystickGetButton(joy,8))
					{
						SDL_Log("L1 pressed\n");
			
					}
					if(SDL_JoystickGetButton(joy,12))
					{
						SDL_Log("L2 pressed\n");
			
					}
					if(SDL_JoystickGetButton(joy,9))
					{
						SDL_Log("R1 pressed\n");
			
					}
					if(SDL_JoystickGetButton(joy,13))
					{
						SDL_Log("R2 pressed\n");
			
					}	
					break;
				case SDL_JOYBUTTONUP: 
					//SDL_Log("SDL button up\n");
					break;
				default:
					break;
			}
		}
	}
}
void updateController()
{
	int ret;
	unsigned int buttons=0;
	
	ret=orbisPadUpdate();
	if(ret==0)
	{
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L2R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L1R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L1|ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"Combo L1R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
			
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R1) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R1) )
		{
			debugNetPrintf(DEBUG,"Combo L2R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_UP) || orbisPadGetButtonHold(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");
			
			if(y-step>=0)
			{
				y=y-step;
			}
			else
			{
				y=0;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			
			if(y+step<conf->height-1)
			{
				y=y+step;
			}
			else
			{
				y=conf->height-1-step;
			}
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			
			if(x+step<conf->width-1)
			{
				x=x+step;
			}
			else
			{
				x=conf->width-1-step;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			
			if(x-step>=0)
			{
				x=x-step;
			}
			else
			{
				x=0;
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed exit\n");
			
			flag=0;
				
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed reset position and color red\n");
			x=1280/2;
			y=720/2;
			color=0x80ff0000;	
			orbisAudioResume(0);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed rand color\n");
			R=rand()%256;
			G=rand()%256;
			B=rand()%256;
			color=0x80000000|R<<16|G<<8|B;
			orbisAudioStop();
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
			orbisAudioPause(0);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			debugNetPrintf(DEBUG,"L1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"R2 pressed\n");
			
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
void initApp()
{
	int ret;

	
	//while(!ps4LinkRequestsIsConnected())
	//{
		
	//}
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
			ret=orbisAudioInitWithConf(myConf->confAudio);
			if(ret==1)
			{
				debugNetPrintf(DEBUG,"libaudio ready\n");
				orbisKeyboardInitWithConf(myConf->confKeyboard);
				
				//ret=orbisAudioInitChannel(ORBISAUDIO_CHANNEL_MAIN,1024,48000,ORBISAUDIO_FORMAT_S16_STEREO);
			}
		}
	}
	
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
	int ret1=-1;
	int ret2=-1;
	ret1=ps4AssemblerSystemCall(47);
	ret2=ps4AssemblerSystemCall(24);
	debugNetPrintf(3,"uid using syscall is %d %d\n",ret2,ret1);
	ret1=-1;
	ret2=-1;
	ret1=getgid();
	ret2=getuid();
	debugNetPrintf(3,"uid using libkernel is %d %d\n",ret2,ret1);
	
	
	
	
	initApp();
	
	
	
	
	Mod_Init(0);
	Mod_Load("host0:zweifeld.mod");
	Mod_Play();
	orbisAudioResume(0);
	
	
	
	
	
	ret=SDL_Init(SDL_INIT_VIDEO);
	if(ret<0)
	{
			SDL_Log("error initializing SDL %s\n",SDL_GetError());
	}
	SDL_Log("SDL is initialized\n");
	sleep(1);
	fillRect.x=x; 
	fillRect.y=y;
	fillRect.w=w;
	fillRect.h=h;				
	SDL_Log("SDL creating window\n");
	if ((gWindow = SDL_CreateWindow( "RedRectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 720, SDL_WINDOW_SHOWN)) == NULL)
		SDL_Log("SDL error %s \n",SDL_GetError());
	    
	SDL_Log("SDL creating renderer\n");
	if ((gRenderer = SDL_CreateRenderer( gWindow, -1, 0)) == NULL)
		SDL_Log("SDL error %s \n",SDL_GetError());
	
	SDL_Log("SDL setting red color\n");
	SDL_SetRenderDrawColor( gRenderer, 0,255,0,0x80);
	
	SDL_Log("SDL creating red rectangle\n");
	SDL_RenderFillRect( gRenderer, &fillRect );
	
	SDL_Log("SDL rendering to display\n");
	SDL_RenderPresent( gRenderer );
	
	
  
	SDL_Log("SDL Joystick init return %d\n",SDL_JoystickInit());
	SDL_Log("SDL num joysticks %d\n",SDL_NumJoysticks());
    // Enumerate joysticks
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {

	/*// Check to see if joystick supports SDL's game controller interface
	if (SDL_IsGameController(i)) {
		controller = SDL_GameControllerOpen(i);
		if (controller) {
			SDL_Log("Found a valid controller, named: %s\n", SDL_GameControllerName(controller));
			break;  // Break after first available controller
		} else {
			SDL_Log("Could not open game controller %i: %s\n", i, SDL_GetError());
		}
	}
	else
	{*/
		joy = SDL_JoystickOpen(i);
		// Joystick is valid
		if (joy) 
		{
			SDL_Log("Opened Joystick %i\n",i);
			SDL_Log("Name: %s\n",SDL_JoystickName(joy));
			SDL_Log("Axes: %d\n",SDL_JoystickNumAxes(joy));
			SDL_Log("Buttons: %d\n",SDL_JoystickNumButtons(joy));
			SDL_Log("Balls: %d\n",SDL_JoystickNumBalls(joy));
		
			// Joystick not valid
		} 
		else 
		{
			SDL_Log("Could not open Joystick %i %s\n",i,SDL_GetError() );
		}
		break;  // Break after first available joystick
	//}
	}
	
	
	
	SDL_Delay(4000);
	
	
	
	
	
	//normal native liborbis sample
	//sleep(5);
	
	
	
	while(flag)
	{
		//capture pad data and populate positions
		// X random color
		// O reset to center position and red color
		// /\ to exit
		// dpad move rectangle
		
		
		
		
		
		
		/* native liborbis code
		//updatekeyboard();
		
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer(1);
				
		//default red is here press X to random color
		orbis2dDrawRectColor(x,w,y,h,color);
				
		//flush and flip
		orbis2dFinishDrawing(flipArg);
				
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;*/
		
		//Sdl equivalent code
		updateControllerSDL();
		
		SDL_SetRenderDrawColor(gRenderer,255,255,255,0x80);
	    SDL_RenderClear(gRenderer);		
		SDL_SetRenderDrawColor(gRenderer, GET_R(color),GET_G(color),GET_B(color),GET_A(color));
		fillRect.x=x;
		fillRect.y=y;
		fillRect.w=w;
		fillRect.h=h;
		SDL_RenderFillRect(gRenderer,&fillRect);
		SDL_RenderPresent(gRenderer);
	
		
		
		
	}
	
	
	SDL_Log("SDL destroy renderer\n");
	SDL_DestroyRenderer( gRenderer );
	
	SDL_Log("SDL destroy window\n");
	SDL_DestroyWindow( gWindow );
	  
	gWindow = NULL;
	gRenderer = NULL;
	
	//end SDL stuff
	
	
	orbisAudioResume(0);
	Mod_End();
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


	exit(EXIT_SUCCESS);
}