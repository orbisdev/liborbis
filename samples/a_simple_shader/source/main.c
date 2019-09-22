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


int x=ATTR_WIDTH /2;
int y=ATTR_HEIGHT/2;
int w=ATTR_WIDTH/64;
int h=ATTR_WIDTH/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
int flag=0;

Orbis2dConfig *conf;
OrbisPadConfig *confPad;


static char refresh = 1;

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

/* to track a random rectangle */
static short rx, rw, ry, rh;
static uint32_t rcolor = 0xFF551199;

static void rr()  // randomize rect
{
	rx = rand()%ATTR_WIDTH,    ry = rand()%ATTR_HEIGHT;
	rw = rand()%ATTR_WIDTH /2, rh = rand()%ATTR_HEIGHT /2;
	rcolor = ARGB(0xFF, rand()%256, rand()%256, rand()%256);
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
			x=ATTR_WIDTH/2;
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
			//orbisAudioStop();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
			orbisAudioPause(0);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			R=rand()%256;
			G=rand()%256;
			B=rand()%256;
			conf->bgColor=0xFF000000|R<<16|G<<8|B;
			debugNetPrintf(DEBUG,"L1 pressed, new random() bgColor: %8x\n", conf->bgColor);
			refresh = 1;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			debugNetPrintf(DEBUG,"L2 pressed\n");
			rr();
			refresh = 1;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			debugNetPrintf(DEBUG,"R1 pressed\n");
			//test();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			debugNetPrintf(DEBUG,"R2 pressed\n");
			
		}
		
			
	}
}
void finishApp()
{
    FT_end();
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
			if(ret!=1)
			{
				debugNetPrintf(DEBUG,"libaudio ready\n");
				
				//ret=orbisAudioInitChannel(ORBISAUDIO_CHANNEL_MAIN,1024,48000,ORBISAUDIO_FORMAT_S16_STEREO);
			}
		}
	}
	
}


uint32_t c1, c2;       // colors to fade text
char tmp_ln[256];  // buffer to store text

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
	FT_init();
	
	Mod_Init(0);
	ret = Mod_Load("host0:main.mod");
	if(ret)
	   Mod_Play();

	orbisAudioResume(0);
	
	// define text fading colors
	c1 = 0xFFFF22AA;
	c2 = 0xFF221133;
	update_gradient(&c1, &c2);  // compute internal fading colors

	sprintf(tmp_ln, "hella ZeraTron!");
	int tx = get_aligned_x(tmp_ln, CENTER);  // center text
	
	rr();

	char *msg[4] = { "hello from a simple shader, but using AVX on CPU", 
	                 "this is a raster implementation",
	                 "#liborbis: where sources awaits",
	                 "GLSL -> raster graphic :facepalm:" };
    uint m = 0;
    	
	while(flag)
	{
		//capture pad data and populate positions
		// X random color
		// O reset to center position and red color
		// /\ to exit
		// dpad move rectangle
		updateController();

		//wait for current display buffer
		orbis2dStartDrawing();

		// clear the current display buffer
		orbis2dClearBuffer(0);  // uses cached dumpBuf

		if(refresh)	// draw the background image
		{
			orbis2dClearBuffer(1);  // don't use dumpBuf, force clean

			// draw a background
			//orbis2dDrawRectColor(rx, rw, ry, rh, rcolor);

			orbis2dDumpBuffer(), refresh = 0;  // save dumpBuf
			debugNetPrintf(DEBUG,"orbis2dDumpBuffer()\n");
		}

    /* test shader math */
    // glsl_e57400();
    // glsl_e57400_sse();
       glsl_e57400_avx();

        // draw text with freetype
        FT_print_text(400, 300, msg[m]);
        
        if(flipArg > 30) { add_angle(); }
        
        if(flipArg % 60 == 0) {
            m += 1;
            if(m > 3) m = 0;
            debugNetPrintf(DEBUG,"flipArg %d, m:%d\n", flipArg, m);
        }

		// draw text with Xbm_Font
		//print_text(tx, ATTR_HEIGHT /2, tmp_ln);

		//default red is here press X to random color
		//orbis2dDrawRectColor(x,w,y,h,color);

		//flush and flip
		orbis2dFinishDrawing(flipArg);
				
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;

		// take a breath, 0.5 microsecond
		sceKernelUsleep(500);
	}
	
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