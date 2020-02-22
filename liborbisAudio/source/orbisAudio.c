/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>  // sleep()
#include <kernel.h>
#include <audioout.h>
#include <debugnet.h>
#include <unistd.h>
#include "orbisAudio.h"



OrbisAudioConfig *orbisAudioConf=NULL;
int orbisaudio_external_conf=-1;

OrbisAudioConfig *orbisAudioGetConf()
{
	if(orbisAudioConf)
	{
		return orbisAudioConf;
	}
	
	return NULL; 
}

int orbisAudioCreateBuffersChannel(unsigned int channel, unsigned int samples, unsigned int format)
{
	int j;
	int size;
	switch(format)
	{
		case ORBISAUDIO_FORMAT_S16_MONO:
			size=sizeof(OrbisAudioMonoSample);
			break;
		case ORBISAUDIO_FORMAT_S16_STEREO:
			size=sizeof(OrbisAudioStereoSample);
			break;
		default:
			break;

	}
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)
		{
			if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized==-1)
			{	
				for(j=0;j<ORBISAUDIO_NUM_BUFFERS;j++)
				{
				
					orbisAudioConf->channels[channel]->sampleBuffer[j]=(short*)malloc(size*samples);
					orbisAudioConf->channels[channel]->samples[j] = samples;

			
				}
				
				orbisAudioConf->channels[channel]->stereo=format;
				debugNetPrintf(DEBUG,"[orbisAudio] buffers for audio channel %d created\n",channel);	
			}			
			else
      			{
				debugNetPrintf(DEBUG,"[orbisAudio] audio channel %d was already initialized\n",channel);
				
			}

			
		}
		else
		{
			return -1;
		}
  	}
	else
	{
		debugNetPrintf(DEBUG,"[orbisAudio] orbisAudioCreateBuffersChannel orbisAudioConf is not created\n");
		return -1;
	}
	return 0;
}

int orbisAudioCreateConf()
{	
	if(orbisAudioConf==NULL)
	{
		orbisAudioConf=(OrbisAudioConfig *)malloc(sizeof(OrbisAudioConfig));
		if(orbisAudioConf!=NULL)
		{
			for(int i=0;i<ORBISAUDIO_CHANNELS;i++)
			{
				orbisAudioConf->channels[i]=malloc(sizeof(OrbisAudioChannel));
				if(orbisAudioConf->channels[i]!=NULL)
				{
					orbisAudioConf->channels[i]->audioHandle=-1;
					orbisAudioConf->channels[i]->threadHandle = 0;
					orbisAudioConf->channels[i]->leftVol = ORBISAUDIO_VOLUME_MAX;
					orbisAudioConf->channels[i]->rightVol = ORBISAUDIO_VOLUME_MAX;
					for(int j=0;j<ORBISAUDIO_NUM_BUFFERS;j++)
					{
						orbisAudioConf->channels[i]->sampleBuffer[j] = NULL;
						orbisAudioConf->channels[i]->samples[j] = 0;
					}
					orbisAudioConf->channels[i]->callback = NULL;
					orbisAudioConf->channels[i]->userData = NULL;
					orbisAudioConf->channels[i]->paused = 1;
					orbisAudioConf->channels[i]->currentBuffer=0;
					orbisAudioConf->channels[i]->orbisaudiochannel_initialized=-1;
				}
			}
			orbisAudioConf->orbisaudio_stop=0;
			orbisAudioConf->orbisaudio_initialized=1;
			return 0;
			
		}

	}
	
	if(orbisAudioConf->orbisaudio_initialized==1)
	{
		return orbisAudioConf->orbisaudio_initialized;
	}
	//something weird happened
	return -1;			
}
int orbisAudioSetConf(OrbisAudioConfig *conf)
{
	if(conf)
	{
		orbisAudioConf=conf;
		orbisaudio_external_conf=1;
		return orbisAudioConf->orbisaudio_initialized;
	}
	
	return 0; 
}
int orbisAudioInitWithConf(OrbisAudioConfig *conf)
{
	int ret;
	
	ret=orbisAudioSetConf(conf);
	if(ret)
	{
		debugNetPrintf(DEBUG,"liborbisaudio already initialized using configuration external\n");
		debugNetPrintf(DEBUG,"orbisaudio_initialized=%d\n",orbisAudioConf->orbisaudio_initialized);
		debugNetPrintf(DEBUG,"ready to have a lot of fun...\n");
		return orbisAudioConf->orbisaudio_initialized;
	}
	else
	{
		return 0;
	}
}
int orbisAudioPlayBlock(unsigned int channel,unsigned int vol1,unsigned int vol2,void *buf)
{

	//int vols[2]={vol1,vol2};
	if (channel > ORBISAUDIO_CHANNELS)
	{
		return 0;
	}
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)
		{
			if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized==1 && orbisAudioConf->orbisaudio_stop!=1)
			{
				//sceAudioOutSetVolume(orbisAudioConf->channels[channel]->audioHandle,ORBISAUDIO_VOLUME_FLAG_LEFT_CHANNEL|ORBISAUDIO_VOLUME_FLAG_RIGHT_CHANNEL,vols);
				return sceAudioOutOutput(orbisAudioConf->channels[channel]->audioHandle,buf);

			}
			
		}
	}
	return -1;

}

void * orbisAudioChannelThread(void *argp)
{
	unsigned int channel=*(unsigned int *)argp;
	int i,ret;
	unsigned short *bufMono;
	unsigned int *bufStereo;
	void *buf;
	OrbisAudioCallback callback=NULL;
	unsigned int samples;
	for(i=0;i<ORBISAUDIO_NUM_BUFFERS;i++)
	{
				
		memset(orbisAudioConf->channels[channel]->sampleBuffer[i],0,orbisAudioConf->channels[channel]->samples[i]*(orbisAudioConf->channels[channel]->stereo?sizeof(OrbisAudioStereoSample):sizeof(OrbisAudioMonoSample))*orbisAudioConf->channels[channel]->samples[i]);
		
	}
	debugNetPrintf(DEBUG,"[orbisAudio] orbisAudioChannelThread %d  %d ready to have a lot of fun!\n",orbisAudioConf->orbisaudio_stop,orbisAudioConf->channels[channel]->paused);
	
	while(!orbisAudioConf->orbisaudio_stop)
	{
		if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized==1)
		{
			callback=orbisAudioConf->channels[channel]->callback;
			buf=orbisAudioConf->channels[channel]->sampleBuffer[orbisAudioConf->channels[channel]->currentBuffer];
			samples=orbisAudioConf->channels[channel]->samples[orbisAudioConf->channels[channel]->currentBuffer];

			if (callback!=NULL && !orbisAudioConf->channels[channel]->paused)
			{
				/* User callback to fill buffer */
				callback(buf,samples,orbisAudioConf->channels[channel]->userData);
			}
			else
			{
				/* Fill buffer with silence */
				if (orbisAudioConf->channels[channel]->stereo==1)
				{
					bufStereo=buf;
					for (i=0;i<samples;i++)
					{
						*(bufStereo++)=0;
					}
				}
				else
				{
					bufMono=buf;
					for(i=0;i<samples;i++)
					{
						*(bufMono++)=0;
					}
				}
			}

			/* Play sound */
			ret=orbisAudioPlayBlock(channel,orbisAudioConf->channels[channel]->leftVol,orbisAudioConf->channels[channel]->rightVol,buf);
			if(ret<0)
			{
				debugNetPrintf(DEBUG,"[orbisAudio] orbisAudioPlayBlock error 0x%08X \n",ret);
			}
			/* Switch active buffer */
			orbisAudioConf->channels[channel]->currentBuffer=(orbisAudioConf->channels[channel]->currentBuffer?0:1);
		}
		sceKernelUsleep(1000);
	}
	
	debugNetPrintf(DEBUG,"[orbisAudio] orbisAudioChannelThread %d  exit...\n",orbisAudioConf->orbisaudio_stop);
	
	scePthreadExit(0);
	return NULL;
		


	
}
void orbisAudioDestroyBuffersChannel(unsigned int channel)
{
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)		
		{
			for(int i=0;i<ORBISAUDIO_NUM_BUFFERS;i++)
			{

				free(orbisAudioConf->channels[channel]->sampleBuffer[i]);
			}
		}					

	}
}
int orbisAudioGetStatus()
{
	if(orbisAudioConf!=NULL)
	{
		return	orbisAudioConf->orbisaudio_initialized;
	}
	else
	{
		return -1;
	}
}
int orbisAudioGetChannelStatus(int chan)
{
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[chan]!=NULL)
		{
			return 	orbisAudioConf->channels[chan]->orbisaudiochannel_initialized;
		}
		return -1;
	}
	else
	{
		return -1;
	}
}
int orbisAudioGetHandle(int chan)
{
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[chan]!=NULL)
		{
			return	orbisAudioConf->channels[chan]->audioHandle;
		}
		return -1;
	}
	else
	{
		return -1;
	}
}
int orbisAudioInitChannelWithoutCallback(unsigned int channel, unsigned int samples, unsigned int frequency, int format)
{
	int ret;
	unsigned int numSamples=0;
	int handle;
	unsigned int localChannel=channel;
	char label[32];
	strcpy(label,"audiotX");
	label[6]='0'+channel;	
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[localChannel]!=NULL)
		{
			if(orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized==-1)
			{
				if (samples<ORBISAUDIO_MIN_LEN)
				{
					numSamples=ORBISAUDIO_MIN_LEN;
				} 
				else
				{
					numSamples=ORBISAUDIO_ALIGN_SAMPLE(samples,ORBISAUDIO_MIN_LEN);
					if(numSamples>ORBISAUDIO_MAX_LEN)
					{
						numSamples=ORBISAUDIO_MAX_LEN;
					}
				}
				ret=orbisAudioCreateBuffersChannel(localChannel,numSamples,format);
				if(ret!=0)
				{
					debugNetPrintf(DEBUG,"[orbisAudio] error creating buffers for audio channel %d\n",localChannel);
					orbisAudioDestroyBuffersChannel(localChannel);
					orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=-1;
				}
				else
				{
					debugNetPrintf(DEBUG,"[orbisAudio] sceAudioOutOpen  %d samples\n",numSamples);
					
					handle=sceAudioOutOpen(0xff,localChannel,0,numSamples,frequency,format);
					if(handle>0)
					{
						orbisAudioConf->channels[localChannel]->audioHandle=handle;	
						orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=1;
						return 0;
					}
					else
					{
						debugNetPrintf(DEBUG,"[orbisAudio] error opening audio channel %d 0x%08X\n",localChannel, handle);
						orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=-1;				
					}
				}
			}		
			else
			{
				debugNetPrintf(DEBUG,"[orbisAudio] audio channel %d already initialized",localChannel);
			}
		}
	}
	return -1;
}
int orbisAudioInitChannel(unsigned int channel, unsigned int samples, unsigned int frequency, int format)
{
	int ret;
	unsigned int numSamples=0;
	int handle;
	unsigned int localChannel=channel;
	char label[32];
	strcpy(label,"audiotX");
	label[6]='0'+channel;	
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[localChannel]!=NULL)
		{
			if(orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized==-1)
			{
				if (samples<ORBISAUDIO_MIN_LEN)
				{
					numSamples=ORBISAUDIO_MIN_LEN;
				} 
				else
				{
					numSamples=ORBISAUDIO_ALIGN_SAMPLE(samples,ORBISAUDIO_MIN_LEN);
					if(numSamples>ORBISAUDIO_MAX_LEN)
					{
						numSamples=ORBISAUDIO_MAX_LEN;
					}
				}
				ret=orbisAudioCreateBuffersChannel(localChannel,numSamples,format);
				if(ret!=0)
				{
					debugNetPrintf(DEBUG,"[orbisAudio] error creating buffers for audio channel %d\n",localChannel);
					orbisAudioDestroyBuffersChannel(localChannel);
					orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=-1;
				}
				else
				{
					debugNetPrintf(DEBUG,"[orbisAudio] sceAudioOutOpen  %d samples\n",numSamples);
					
					handle=sceAudioOutOpen(0xff,localChannel,0,numSamples,frequency,format);
					if(handle>0)
					{
						orbisAudioConf->channels[localChannel]->audioHandle=handle;	
			
						ret=scePthreadCreate(&orbisAudioConf->channels[localChannel]->threadHandle,NULL,orbisAudioChannelThread, &localChannel,label);
						if(ret==0)
						{
							debugNetPrintf(DEBUG,"[orbisAudio] audio channel %d thread UID: 0x%08X created\n",localChannel, orbisAudioConf->channels[localChannel]->threadHandle);


							orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=1;
							return 0;


						}
						else
						{
							debugNetPrintf(DEBUG,"[orbisAudio] audio channel %d thread could not create error: 0x%08X\n",localChannel, ret);
							scePthreadCancel(orbisAudioConf->channels[localChannel]->threadHandle);
							debugNetPrintf(DEBUG,"[orbisAudio] closing audio channel %d\n",channel);
							sceAudioOutClose(orbisAudioConf->channels[channel]->audioHandle);
							orbisAudioConf->channels[channel]->audioHandle=-1;
							orbisAudioDestroyBuffersChannel(channel);
							orbisAudioConf->channels[channel]->orbisaudiochannel_initialized=-1;
						
						}
		
					
					}
					else
					{
						debugNetPrintf(DEBUG,"[orbisAudio] error opening audio channel %d 0x%08X\n",localChannel, handle);
						orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=-1;
					
					}

				}
			}
		
			else
			{
				debugNetPrintf(DEBUG,"[orbisAudio] audio channel %d already initialized",localChannel);
			}
		}
	}
	return -1;
	


}

int orbisAudioSetCallback(unsigned int channel,OrbisAudioCallback callback,void *userdata)
{
	if (channel > ORBISAUDIO_CHANNELS)
	{
		return 0;
	}
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)
		{
			orbisAudioConf->channels[channel]->callback=NULL;
			orbisAudioConf->channels[channel]->userData = userdata;
			orbisAudioConf->channels[channel]->callback = callback;
		}
	}
	return 1;
}
int orbisAudioStop()
{
	if(orbisAudioConf!=NULL)
	{
		
		orbisAudioConf->orbisaudio_stop=1;
		sleep(2);
		
	}
	return 1;
}
int orbisAudioResume(unsigned int channel)
{
	if (channel > ORBISAUDIO_CHANNELS)
	{
		return 0;
	}
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)
		{
			orbisAudioConf->channels[channel]->paused=0;
		}
	}

  	return 1;
}

int orbisAudioPause(unsigned int channel)
{
	if (channel > ORBISAUDIO_CHANNELS)
	{
		return 0;
	}
	if(orbisAudioConf!=NULL)
	{
		if(orbisAudioConf->channels[channel]!=NULL)
		{
			orbisAudioConf->channels[channel]->paused=1;
		}
	}

  	return 1;
}
void orbisAudioFinish()
{
	int i;
	if(orbisAudioConf!=NULL)
	{
		orbisAudioStop();
		for(i=0;i<ORBISAUDIO_CHANNELS;i++)
		{
			if(orbisAudioConf->channels[i]!=NULL)
			{					
				if(orbisAudioConf->channels[i]->orbisaudiochannel_initialized==1)
				{
					
					//scePthreadCancel(orbisAudioConf->channels[i]->threadHandle);
					orbisAudioConf->channels[i]->threadHandle=0;
					sceAudioOutClose(orbisAudioConf->channels[i]->audioHandle);
					debugNetPrintf(DEBUG,"[orbisAudio] closing audio handle\n");
					
					orbisAudioConf->channels[i]->audioHandle=-1;
					//orbisAudioDestroyBuffersChannel(i);
					debugNetPrintf(DEBUG,"[orbisAudio] free buffers channel\n");
					
					orbisAudioConf->channels[i]->orbisaudiochannel_initialized=-1;
					
				}
	
				//free(orbisAudioConf->channels[i]);
			}				
		}
		//free(orbisAudioConf);
		debugNetPrintf(DEBUG,"[orbisAudio] finished\n");
		
	}
}
int orbisAudioInit()
{
	int ret;
	ret=sceAudioOutInit();
	if(ret<0)
	{
		debugNetPrintf(DEBUG,"[orbisAudio] sceAudioOutInit error 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(DEBUG,"[orbisAudio] sceAudioOutInit return %d\n",ret);
	if(orbisAudioCreateConf()==0)
	{
		debugNetPrintf(DEBUG,"[orbisAudio] initialized!\n");
		return orbisAudioConf->orbisaudio_initialized;
	}
	if (orbisAudioConf->orbisaudio_initialized==1) 
	{
		debugNetPrintf(DEBUG,"[orbisAudio] is already initialized!\n");
		return orbisAudioConf->orbisaudio_initialized;
	}
	return -1;
}
