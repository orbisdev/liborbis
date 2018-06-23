/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <userservice.h>
#include <pad.h>
#include <debugnet.h>
#include "orbisPad.h"

OrbisPadConfig *orbisPadConf=NULL;

int orbispad_external_conf=-1;

void orbisPadFinish()
{
	int ret;
	if(orbispad_external_conf!=1)
	{
		if(orbisPadConf->orbispad_initialized==1)
		{		
			ret=scePadClose(orbisPadConf->padHandle);
		
			debugNetPrintf(DEBUG,"liborbis2d scePadClose return 0x%8x\n",ret);
		}
		orbisPadConf->orbispad_initialized=-1;
	
		debugNetPrintf(DEBUG,"liborbisPad finished");
	}
}

OrbisPadConfig *orbisPadGetConf()
{
	if(orbisPadConf)
	{
		return orbisPadConf;
	}
	
	return NULL; 
}
int orbisPadCreateConf()
{	
	if(!orbisPadConf)
	{
		orbisPadConf=(OrbisPadConfig *)malloc(sizeof(OrbisPadConfig));
		
		orbisPadConf->userId=0;
		orbisPadConf->padHandle=-1;
		
		orbisPadConf->padDataCurrent=(ScePadData *)malloc(sizeof(ScePadData));
		orbisPadConf->padDataLast=(ScePadData *)malloc(sizeof(ScePadData));
		
		orbisPadConf->buttonsPressed=0;
		orbisPadConf->buttonsReleased=0;
		orbisPadConf->buttonsHold=0;
		
		orbisPadConf->orbispad_initialized=-1;
		
		return 0;
	}
	
	if(orbisPadConf->orbispad_initialized==1)
	{
		return orbisPadConf->orbispad_initialized;
	}
	//something weird happened
	return -1;			
}
int orbisPadSetConf(OrbisPadConfig *conf)
{
	if(conf)
	{
		orbisPadConf=conf;
		orbispad_external_conf=1;
		return orbisPadConf->orbispad_initialized;
	}
	
	return 0; 
}
int orbisPadInitWithConf(OrbisPadConfig *conf)
{
	int ret;
	ret=orbisPadSetConf(conf);
	if(ret)
	{
		debugNetPrintf(DEBUG,"liborbisPad already initialized using configuration external\n");
		debugNetPrintf(DEBUG,"orbispad_initialized=%d\n",orbisPadConf->orbispad_initialized);
		debugNetPrintf(DEBUG,"ready to have a lot of fun...\n");
		return orbisPadConf->orbispad_initialized;
	}
	else
	{
		return 0;
	}
}
unsigned int orbisPadGetCurrentButtonsPressed()
{
	return orbisPadConf->buttonsPressed;
}
void orbisPadSetCurrentButtonsPressed(unsigned int buttons)
{
	orbisPadConf->buttonsPressed=buttons;
}
unsigned int orbisPadGetCurrentButtonsReleased()
{
	return orbisPadConf->buttonsReleased;
}
void orbisPadSetCurrentButtonsReleased(unsigned int buttons)
{
	orbisPadConf->buttonsReleased=buttons;
}

bool orbisPadGetButtonHold(unsigned int filter)
{
	if((orbisPadConf->buttonsHold&filter)==filter)
	{
		return 1;
	}
	return 0;
}
bool orbisPadGetButtonPressed(unsigned int filter)
{
	if((orbisPadConf->buttonsPressed&filter)==filter)
	{
		return 1;
	}
	return 0;
}
bool orbisPadGetButtonReleased(unsigned int filter)
{
 	if((orbisPadConf->buttonsReleased&filter)==filter)
	{
		if(~(orbisPadConf->padDataLast->buttons)&filter)
		{
			return 0;
		}
		return 1;
	}
	return 0;
}
int orbisPadUpdate()
{
	int ret;
	unsigned int actualButtons=0;
	unsigned int lastButtons=0;
	memcpy(orbisPadConf->padDataLast,orbisPadConf->padDataCurrent,sizeof(ScePadData));
	
	ret=scePadReadState(orbisPadConf->padHandle,orbisPadConf->padDataCurrent);
	
	if(ret==0 && orbisPadConf->padDataCurrent->connected==1)
	{
		actualButtons=orbisPadConf->padDataCurrent->buttons;
		lastButtons=orbisPadConf->padDataLast->buttons;
		orbisPadConf->buttonsPressed=(actualButtons)&(~lastButtons);
		if(actualButtons!=lastButtons)
		{
			orbisPadConf->buttonsReleased=(~actualButtons)&(lastButtons);
		}
		else
		{
			orbisPadConf->buttonsReleased=0;
			
		}
		orbisPadConf->buttonsHold=actualButtons&lastButtons;
		return 0;
		
	}
	else
	{
		return -1;
	}
}
int orbisPadInit()
{
    int ret;
   
    int param=700;
	
	if(orbisPadCreateConf()==1)
	{
			return orbisPadConf->orbispad_initialized;
	}
	if (orbisPadConf->orbispad_initialized==1) 
	{
		debugNetPrintf(DEBUG,"liborbispad is already initialized!\n");
		return orbisPadConf->orbispad_initialized;
	}
	ret=sceUserServiceInitialize(&param);
	debugNetPrintf(DEBUG,"liborbispad sceUserServiceInitialize return 0x%8x!\n",ret);
	
       
    if(ret==0)
    {
        ret=scePadInit();
        if(ret<0)
        {
            debugNetPrintf(DEBUG,"liborbispad scePadInit return error 0x%8x\n",ret);
            return -1;
        }
        debugNetPrintf(DEBUG,"liborbispad scePadInit return %d\n",ret);
        if(ret==0)
        {
           
            ret=sceUserServiceGetInitialUser(&orbisPadConf->userId);
			if(ret<0)
			{
	            debugNetPrintf(DEBUG,"liborbispad sceUserServiceGetInitialUser return error 0x%8x\n",ret);
				orbisPadConf->userId=0x10000000;
				
			}
			
            orbisPadConf->padHandle=scePadOpen(orbisPadConf->userId, 0, 0, NULL);
            if(orbisPadConf->padHandle<0)
            {
                debugNetPrintf(DEBUG,"liborbispad scePadOpen return error 0x%8x\n",orbisPadConf->padHandle);
                return -1;
            }
            debugNetPrintf(DEBUG,"liborbispad scePadOpen return handle 0x%8x\n",orbisPadConf->padHandle);
            if(orbisPadConf->padHandle>0)
            {
                    orbisPadConf->orbispad_initialized=1;
                
            }

        }
    }
   
    return orbisPadConf->orbispad_initialized;
}

