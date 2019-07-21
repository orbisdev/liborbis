#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <orbis2d.h>
#include <orbisPad.h>
#include <orbisKeyboard.h>
#include <orbisAudio.h>
#include <ps4link.h>
#include <debugnet.h>
#include <orbisXbmFont.h>
#include <orbisFile.h>


int x=ATTR_WIDTH /2;
int y=ATTR_HEIGHT/2;
int w=ATTR_WIDTH/64;
int h=ATTR_WIDTH/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
int flag=0;

Orbis2dConfig  *conf;
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

static void rr()  // randomize rectangle
{
    rx = rand()%ATTR_WIDTH,    ry = rand()%ATTR_HEIGHT;
    rw = rand()%ATTR_WIDTH /2, rh = rand()%ATTR_HEIGHT /2;
    rcolor = ARGB(0xFF, rand()%256, rand()%256, rand()%256);
}

#include "minimp3_test.h"  // mp3 decoder

#include "menu.h"   // menu part
v4i pos = (0);      // file selection in menu

#include "freetype.h"  // uses FT_

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

            switch(pos.z)
            {
                case MAIN:
                    pos.w   = ORBISPAD_UP;
                    refresh = browserUpdateController(&pos);
                    break;
                default:
                    y = (y-step>=0) ? y-step : 0;
                    break;
            }
        }
        if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
        {
            debugNetPrintf(DEBUG,"Down pressed\n");

            switch(pos.z)
            {
                case MAIN:
                    pos.w   = ORBISPAD_DOWN;
                    refresh = browserUpdateController(&pos);
                    break;
                default:
                    y = (y+step<conf->height-1) ? y+step : conf->height-1-step;
                    break;
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

            flag=0;  // exit app
        }
        if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
        {
            debugNetPrintf(DEBUG,"Circle pressed reset position and color red\n");

            switch(pos.z)
            {
                case MAIN:
                    pos.w   = ORBISPAD_CIRCLE;
                    refresh = browserUpdateController(&pos);
                    break;
                default:
                    x=1280/2;
                    y=720/2;
                    color=0x80ff0000;
                    orbisAudioResume(0);
                    break;
            }
        }
        if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
        {
            debugNetPrintf(DEBUG,"Cross pressed rand color\n");

            switch(pos.z)
            {
                case MAIN:
                    pos.w   = ORBISPAD_CROSS;
                    refresh = browserUpdateController(&pos);                    //enterDir(&pos); refresh = 1;
                    break;
                default:
                    R=rand()%256;
                    G=rand()%256;
                    B=rand()%256;
                    color=0x80000000|R<<16|G<<8|B;
                    //orbisAudioStop();

                    //test();
                    break;
            }

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
            //rr(); refresh = 1;
        }
        if(orbisPadGetButtonPressed(ORBISPAD_R1))
        {
            debugNetPrintf(DEBUG,"R1 pressed\n");
        }
        if(orbisPadGetButtonPressed(ORBISPAD_R2))
        {
            debugNetPrintf(DEBUG,"R2 pressed, %d %d %d\n", pos.x, pos.y, pos.z);
            pos.z = (pos.z != MAIN) ? MAIN : CLOSED;
            refresh = 1;
        }
    }
}

void finishApp()
{
    FT_end();

    orbisAudioFinish();
    orbisPadFinish();
    orbisFileFinish();
    orbisFileBrowserFinish();
    orbis2dFinish();
    ps4LinkFinish();
}

void initApp()
{
    int ret;
    debugNetPrintf(DEBUG,"[PS4LINK] Initialized and connected from pc/mac ready to receive commands\n");

    //hide playroom splash
    sceSystemServiceHideSplashScreen();
    //more library initialiazation here pad,filebroser,audio,keyboard, etc
    //....
    orbisFileInit();
    orbisFileBrowserInit("host0:");

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

    FT_init();
}


uint32_t c1, c2;   // colors to fade text
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

    minimp3_Init(0);
    ret =   minimp3_Load("host0:outputfile.mp3");
    if(ret) minimp3_Play();
    orbisAudioResume(0);

    // define text fading colors
    c1 = 0xFFFF22AA;
    c2 = 0xFF221133;
    update_gradient(&c1, &c2);  // compute internal fading colors

    sprintf(tmp_ln, "minimp3 liborbis demo");
    int tx = get_aligned_x(tmp_ln, CENTER);  // center text

    //rr();
    conf->bgColor=0xFF200022;

    while(flag)
    {
        /*if(flipArg %2 == 0)
        {
          conf->bgColor=get_new_color(); //refresh=1;
        }*/

        // capture pad data and populate positions
        // X random color
        // O reset to center position and red color
        // /\ to exit
        // dpad move rectangle
        updateController();

        //wait for current display buffer
        orbis2dStartDrawing();

        if(refresh) // draw the background image
        {
            orbis2dClearBuffer(1);  // don't use dumpBuf, force clean

            // draw a background
            orbis2dDrawRectColor(rx, rw, ry, rh, rcolor);
            
            switch(pos.z)
            {
              case MAIN:
                  browserDraw(&pos);
                  break;

              default:
                  for(int i =12; i<17; i++)
                  {
                      c1 = 0xFFEEDDFF, c2 = 0xFF221133;
                      FT_update_gradient(&c1, &c2);  // compute internal fading colors
                      FT_set_text_size(i, 100);
                      sprintf(tmp_ln, "%dpt/100dpi: a quick brown fox jumps over the lazy dog !@#$%&/()=?^", i);
                      tx = FT_get_text_lenght(ATTR_WIDTH /2, ATTR_HEIGHT /2, tmp_ln);
                      FT_print_text((ATTR_WIDTH - tx) /2, 50 + (i-8) *29, tmp_ln);
                  }
                  //print_text(tx, ATTR_HEIGHT /2, tmp_ln);  // draw text with Xbm_Font
                  break;
            }

            orbis2dDumpBuffer(), refresh = 0;  // save dumpBuf
            debugNetPrintf(DEBUG,"orbis2dDumpBuffer()\n");
        }
        else
        {
            // clear the current display buffer
            orbis2dClearBuffer(0);  // uses cached dumpBuf
        }
        
        switch(pos.z)
        {
          case MAIN:
              browserDrawSelection(&pos);
              break;
          default:
              orbis2dDrawRectColor(x, w, y, h, color); // default red, press X to random color
              break;
        }
        
        // flush and flip
        orbis2dFinishDrawing(flipArg);

        // swap buffers
        orbis2dSwapBuffers();
        flipArg++;

        // take a breath, 10*1 microsecond
        sceKernelUsleep(10000);
    }

    orbisAudioResume(0);
    minimp3_End();

    // wait for current display buffer
    orbis2dStartDrawing();

    // clear with background (default white) to the current display buffer
    orbis2dClearBuffer(1);

    // flush and flip
    orbis2dFinishDrawing(flipArg);

    // swap buffers
    orbis2dSwapBuffers();

    finishApp();

    myConf->orbisLinkFlag=1;


    exit(EXIT_SUCCESS);
}
