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
#include <pthread.h>

#include "orbisAudio.h"

#if defined __PS4__
#include <sce/kernel.h>
#include <audioout.h>
#include <debugnet.h>
#define  fprintf  debugNetPrintf
/*
#define INFO  1 stdout
#define ERROR 2 stderr
#define DEBUG 3
*/
#elif defined HAVE_LIBAO
// we wrap sce calls below to use default audio device!
#define  sceKernelUsleep  usleep
#include <ao/ao.h>
ao_device *device         = NULL;
int        default_driver = -1;
ao_sample_format format;

// -- libao init --
int sceAudioOutInit(void)
{
    // -- Setup for default audio driver --
    ao_initialize();
    // alsa pulls pulseaudio-alsa-plugin!
    default_driver     = ao_driver_id("alsa"); // or let 'ao_default_driver_id();' autoselect default
    format.bits        = 16;
    format.channels    = 2;
    format.rate        = 48000;
    format.byte_format = AO_FMT_LITTLE;

    return 1;
}

// -- libao open driver --
int sceAudioOutOpen(int unused, int localChannel, int zero, int numSamples, int frequency, int unused_format)
{
    device = ao_open_live(default_driver, &format, NULL );
    if(device == NULL) { fprintf(2, "Error opening device.\n"); return -1; }

    return default_driver;
}

// -- libao play audio --
int sceAudioOutOutput(int audioHandle, void *buf)
{
    ao_play(device, (char *)buf, 1024 *2 * sizeof(short)); return 1;
}

// -- libao end --
void sceAudioOutClose(int audioHandle)
{
    ao_close(device);
    ao_shutdown();
}
#endif


//static pthread_mutex_t wait_mutex;
static unsigned int localChannel = 0;

OrbisAudioConfig *orbisAudioConf=NULL;
int orbisaudio_external_conf=-1;

OrbisAudioConfig *orbisAudioGetConf()
{
    if(orbisAudioConf) return orbisAudioConf;
    else               return NULL; 
}

int orbisAudioCreateBuffersChannel(unsigned int channel, unsigned int samples, unsigned int format)
{
    int size = 0;
    switch(format)
    {
        case ORBISAUDIO_FORMAT_S16_MONO:   size = sizeof(OrbisAudioMonoSample);   break;
        case ORBISAUDIO_FORMAT_S16_STEREO: size = sizeof(OrbisAudioStereoSample);
        default: break;
    }
    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[channel])
        {
            if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized == -1)
            {
                for(int i=0; i<ORBISAUDIO_NUM_BUFFERS; i++) // double buffers
                {
                    orbisAudioConf->channels[channel]->sampleBuffer[i] = (short*)malloc(size * samples);
                    orbisAudioConf->channels[channel]->samples     [i] = samples;
                    fprintf(1, "[orbisAudio] buffer %d for audio channel %d created (%db)\n", i, channel, size * samples);  
                }
                orbisAudioConf->channels[channel]->stereo = format;
                fprintf(1, "setting format:%d\n", format);
            }
            else fprintf(1, "[orbisAudio] audio channel %d was already initialized\n", channel);
        }
        else return -1;
    }
    else { fprintf(2, "[orbisAudio] orbisAudioCreateBuffersChannel orbisAudioConf is not created\n"); return -1;    }

    return 0;
}

int orbisAudioCreateConf()
{   
    if(!orbisAudioConf)
    {
        orbisAudioConf = (OrbisAudioConfig *)malloc(sizeof(OrbisAudioConfig));
        memset(orbisAudioConf, 0, sizeof(OrbisAudioConfig));
        if(orbisAudioConf)
        {
            for(int i=0;i<ORBISAUDIO_CHANNELS;i++)
            {
                orbisAudioConf->channels[i] = (OrbisAudioChannel *)malloc(sizeof(OrbisAudioChannel));
                memset(orbisAudioConf->channels[i], 0, sizeof(OrbisAudioChannel));
                if(orbisAudioConf->channels[i])
                {
                    orbisAudioConf->channels[i]->audioHandle  = -1;
                    orbisAudioConf->channels[i]->threadHandle =  0;
                    orbisAudioConf->channels[i]->leftVol      = ORBISAUDIO_VOLUME_MAX;
                    orbisAudioConf->channels[i]->rightVol     = ORBISAUDIO_VOLUME_MAX;
                    for(int j=0;j<ORBISAUDIO_NUM_BUFFERS;j++)
                    {
                        orbisAudioConf->channels[i]->sampleBuffer[j] = NULL;
                        orbisAudioConf->channels[i]->samples     [j] = 0;
                    }
                    orbisAudioConf->channels[i]->callback      = NULL;
                    orbisAudioConf->channels[i]->userData      = NULL;
                    orbisAudioConf->channels[i]->paused        = 1;
                    orbisAudioConf->channels[i]->currentBuffer = 0;
                    orbisAudioConf->channels[i]->orbisaudiochannel_initialized = -1;
                }
            }
            orbisAudioConf->orbisaudio_stop        = 0;
            orbisAudioConf->orbisaudio_initialized = 1;
            return 0;
        }
    }
    if(orbisAudioConf->orbisaudio_initialized == 1) return 1;

    return -1; // something weird happened
}

int orbisAudioSetConf(OrbisAudioConfig *conf)
{
    if(conf)
    {
        orbisAudioConf           = conf;
        orbisaudio_external_conf = 1;
        return orbisAudioConf->orbisaudio_initialized;
    }
    
    return 0; 
}
int orbisAudioInitWithConf(OrbisAudioConfig *conf)
{
    int ret = orbisAudioSetConf(conf);
    if(ret)
    {
        fprintf(1, "liborbisaudio already initialized using configuration external\n");
        fprintf(1, "orbisaudio_initialized=%d\n", orbisAudioConf->orbisaudio_initialized);
        fprintf(1, "ready to have a lot of fun...\n");
        return orbisAudioConf->orbisaudio_initialized;
    }
    else return 0;
}

int orbisAudioPlayBlock(unsigned int channel,unsigned int vol1,unsigned int vol2,void *buf)
{
    //int vols[2]={vol1,vol2};
    if (channel > ORBISAUDIO_CHANNELS) return 0;

    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[channel])
        {
            if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized == 1
            && orbisAudioConf->orbisaudio_stop != 1)
            {
                // sceAudioOutSetVolume(orbisAudioConf->channels[channel]->audioHandle,ORBISAUDIO_VOLUME_FLAG_LEFT_CHANNEL|ORBISAUDIO_VOLUME_FLAG_RIGHT_CHANNEL,vols);
                return sceAudioOutOutput(orbisAudioConf->channels[channel]->audioHandle, buf);
            }
        }
    }
    return -1;
}

void * orbisAudioChannelThread(void *argp)
{
    fprintf(1, "-- audio thread --\n");
    int i,ret;

    OrbisAudioCallback callback = NULL;
    unsigned short    *bufMono;
    unsigned short    *bufStereo;
    void              *buf;
    unsigned int       samples;
    unsigned int       channel = *((unsigned int*)argp);
long *p = (long*)argp;
fprintf(2,"localChannel:%ld %d '%.16x' %p %lx %zu %.16x, %.16x\n", *p, (int)channel, channel, argp, (long)argp, sizeof(long), *(int*)argp, *((int*)argp));
if(channel != 0
|| channel != localChannel)
{
    fprintf(2, "wtf! localChannel:%u '%.8x' %p %p %.8lx\n", channel, channel, argp, argp, *(long*)argp);
    channel = 0; sleep(2);
}
///static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
    for(i=0; i<ORBISAUDIO_NUM_BUFFERS; i++)
    {
        size_t size = 0;
//fprintf(1, "\n%u %p, %u, i:%d, size: %zu, %p\n", channel, argp, *(unsigned int*)argp, i, size, orbisAudioConf);

//fprintf(1, "i:%d stereo:%d\n", i, orbisAudioConf->channels[channel]->stereo);
        switch(orbisAudioConf->channels[channel]->stereo)
        {
            case  0: size = sizeof(OrbisAudioMonoSample);   break;
            case  1: size = sizeof(OrbisAudioStereoSample);
            default: break;
        }
    //fprintf(1, "size: %zu * %d\n", size, orbisAudioConf->channels[channel]->samples[i]);
        size *= orbisAudioConf->channels[channel]->samples[i];
//fprintf(1, "i:%d, size: %zu %p\n", i, size, orbisAudioConf->channels[channel]->sampleBuffer[i]);
//pthread_mutex_lock(&wait_mutex);
        memset(orbisAudioConf->channels[channel]->sampleBuffer[i], 0, size);
//pthread_mutex_unlock(&wait_mutex);
    }
    //pthread_mutex_unlock(&wait_mutex);

    fprintf(1, "[orbisAudio] orbisAudioChannelThread %d %d ready to have a lot of fun!\n", orbisAudioConf->orbisaudio_stop, orbisAudioConf->channels[channel]->paused);
    orbisAudioConf->orbisaudio_stop = 0; // XXX reset it

    while(!orbisAudioConf->orbisaudio_stop)
    {
        if(orbisAudioConf->channels[channel]->orbisaudiochannel_initialized == 1)
        {
            callback = orbisAudioConf->channels[channel]->callback;
            buf      = orbisAudioConf->channels[channel]->sampleBuffer[orbisAudioConf->channels[channel]->currentBuffer];
            samples  = orbisAudioConf->channels[channel]->samples     [orbisAudioConf->channels[channel]->currentBuffer];

            if(callback && !orbisAudioConf->channels[channel]->paused)
            {
                /* User callback to fill buffer */
                callback(buf,samples,orbisAudioConf->channels[channel]->userData);
            }
            else
            {
                /* Fill buffer with silence */
                if (orbisAudioConf->channels[channel]->stereo == 1)
                {
                    bufStereo = buf; for(i=0; i<samples; i++) { *(bufStereo++) = 0; }
                }
                else
                {
                    bufMono = buf;   for(i=0; i<samples; i++) { *(bufMono++) = 0; }
                }
            }

            /* Play sound */
            ret = orbisAudioPlayBlock(channel,orbisAudioConf->channels[channel]->leftVol,orbisAudioConf->channels[channel]->rightVol,buf);
            if(ret<0)
            {
                fprintf(2, "[orbisAudio] orbisAudioPlayBlock error 0x%08X \n",ret);
            }
            /* Switch active buffer */
            orbisAudioConf->channels[channel]->currentBuffer=(orbisAudioConf->channels[channel]->currentBuffer?0:1);
        }
        sceKernelUsleep(1000);
    }    
    fprintf(1, "[orbisAudio] stop:%d, orbisAudioChannelThread exit...\n",orbisAudioConf->orbisaudio_stop);
    //scePthreadExit(0);
    pthread_join(orbisAudioConf->channels[channel]->threadHandle, NULL);

    return NULL;
}

void orbisAudioDestroyBuffersChannel(unsigned int channel)
{
    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[channel])
        {
            for(int i=0;i<ORBISAUDIO_NUM_BUFFERS;i++)
            {
// XXX to_fix;         if(orbisAudioConf->channels[channel]->sampleBuffer[i]) free(orbisAudioConf->channels[channel]->sampleBuffer[i]);
            }
        }
    }
}

int orbisAudioGetStatus()
{
    if(orbisAudioConf)
    {
        return orbisAudioConf->orbisaudio_initialized;
    }
    else return -1;
}

int orbisAudioGetChannelStatus(int chan)
{
    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[chan])
        {
            return  orbisAudioConf->channels[chan]->orbisaudiochannel_initialized;
        }
        return -1;
    }
    else return -1;
}

int orbisAudioGetHandle(int chan)
{
    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[chan])
        {
            return orbisAudioConf->channels[chan]->audioHandle;
        }
        return -1;
    }
    else return -1;
}

int orbisAudioInitChannelWithoutCallback(unsigned int channel, unsigned int samples, unsigned int frequency, int format)
{
    int ret;
    unsigned int numSamples = 0;
    int handle;
    localChannel = channel;
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
                    numSamples = ORBISAUDIO_ALIGN_SAMPLE(samples,ORBISAUDIO_MIN_LEN);

                    if(numSamples > ORBISAUDIO_MAX_LEN) numSamples = ORBISAUDIO_MAX_LEN;
                }
                ret = orbisAudioCreateBuffersChannel(localChannel,numSamples,format);
                if(ret!=0)
                {
                    fprintf(2, "[orbisAudio] error creating buffers for audio channel %d\n",localChannel);
                    orbisAudioDestroyBuffersChannel(localChannel);
                    orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=-1;
                }
                else
                {
                    fprintf(1, "[orbisAudio] sceAudioOutOpen %d samples\n",numSamples);
                    
                    handle=sceAudioOutOpen(0xff,localChannel,0,numSamples,frequency,format);
                    fprintf(1, "handle: %d\n", handle);
                    if(handle>0)
                    {
                        orbisAudioConf->channels[localChannel]->audioHandle=handle; 
                        orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized=1;
                        return 0;
                    }
                    else
                    {
                        fprintf(2, "[orbisAudio] error opening audio channel %d 0x%08X\n",localChannel, handle);
                        orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized = -1;
                    }
                }
            }
            else
            {
                fprintf(1, "[orbisAudio] audio channel %d already initialized",localChannel);
            }
        }
    }
    return -1;
}
int orbisAudioInitChannel(unsigned int channel, unsigned int samples, unsigned int frequency, int format)
{
    localChannel = channel;

    unsigned int numSamples = 0;
    int ret, handle;
    char label[32];
    strcpy(label, "audiotX");
    label[6] = '0' + channel;

    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[localChannel])
        {
            if(orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized==-1)
            {
                if(samples<ORBISAUDIO_MIN_LEN) numSamples = ORBISAUDIO_MIN_LEN; 
                else
                {
                    numSamples = ORBISAUDIO_ALIGN_SAMPLE(samples, ORBISAUDIO_MIN_LEN);

                    if(numSamples>ORBISAUDIO_MAX_LEN) numSamples = ORBISAUDIO_MAX_LEN;
                }

                ret = orbisAudioCreateBuffersChannel(localChannel, numSamples, format);
                if(ret)
                {
                    fprintf(2, "[orbisAudio] error creating buffers for audio channel %d\n", localChannel);
                    orbisAudioDestroyBuffersChannel(localChannel);
                    orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized = -1;
                }
                else
                {
                    fprintf(1, "[orbisAudio] sceAudioOutOpen %d samples\n",numSamples);
                    
                    handle=sceAudioOutOpen(0xff,localChannel,0,numSamples,frequency,format);
//fprintf(1, "handle:%d\n", handle);
                    if(handle>0)
                    {
                        // protect the localChannel variable
    //static pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;

                        orbisAudioConf->channels[localChannel]->audioHandle=handle;

                        //ret=scePthreadCreate(&orbisAudioConf->channels[localChannel]->threadHandle,NULL,orbisAudioChannelThread, &localChannel,label);
                        ret = pthread_create(&orbisAudioConf->channels[localChannel]->threadHandle,
                                 NULL,
                                 orbisAudioChannelThread,
                                 (void *)&localChannel);

                        //pthread_mutex_lock(&wait_mutex);
//fprintf(1, "creating thread res:%d\n", ret);
    //pthread_mutex_unlock(&wait_mutex);
    //pthread_mutex_destroy(&wait_mutex);
//usleep(2000);
                        if(ret==0)
                        {
                            fprintf(1, "[orbisAudio] audio channel %u thread UID: 0x%08lX created\n", localChannel, orbisAudioConf->channels[localChannel]->threadHandle);

                            orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized = 1;
                            return 0;
                        }
                        else
                        {
                            fprintf(2, "[orbisAudio] audio channel %u thread could not create error: 0x%08X\n",localChannel, ret);
                            //scePthreadCancel(orbisAudioConf->channels[localChannel]->threadHandle);
                            pthread_join(orbisAudioConf->channels[localChannel]->threadHandle, NULL);

                            fprintf(1, "[orbisAudio] closing audio channel %d\n", channel);

                            sceAudioOutClose(orbisAudioConf->channels[channel]->audioHandle);

                            orbisAudioConf->channels[channel]->audioHandle = -1;
                            orbisAudioDestroyBuffersChannel(channel);
                            orbisAudioConf->channels[channel]->orbisaudiochannel_initialized = -1;
                        }

                    }
                    else
                    {
                        fprintf(2, "[orbisAudio] error opening audio channel %u 0x%08X\n",localChannel, handle);
                        orbisAudioConf->channels[localChannel]->orbisaudiochannel_initialized = -1;
                    }
                }
            }
            else
            {
                fprintf(1, "[orbisAudio] audio channel %u already initialized",localChannel);
            }
        }
    }
    return -1;
}

int orbisAudioSetCallback(unsigned int channel,OrbisAudioCallback callback,void *userdata)
{
    if(channel > ORBISAUDIO_CHANNELS) return 0;

    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[channel])
        {
            orbisAudioConf->channels[channel]->callback = NULL;
            orbisAudioConf->channels[channel]->userData = userdata;
            orbisAudioConf->channels[channel]->callback = callback;
        }
    }
    return 1;
}

int orbisAudioStop()
{
    if(orbisAudioConf) orbisAudioConf->orbisaudio_stop = 1; sleep(1);

    return 1;
}

int orbisAudioResume(unsigned int channel)
{
    if (channel > ORBISAUDIO_CHANNELS) return 0;

    if(orbisAudioConf)
    {
        if(orbisAudioConf->channels[channel]) orbisAudioConf->channels[channel]->paused = 0;
    }
    return 1;
}

int orbisAudioPause(unsigned int channel)
{
    if (channel > ORBISAUDIO_CHANNELS) return 0;

    if(orbisAudioConf!=NULL)
    {
        if(orbisAudioConf->channels[channel]) orbisAudioConf->channels[channel]->paused = 1;
    }
    return 1;
}

void orbisAudioFinish()
{
    //pthread_mutex_destroy(&wait_mutex);

    int i;
    if(orbisAudioConf)
    {
        orbisAudioStop();
        for(i=0;i<ORBISAUDIO_CHANNELS;i++)
        {
            if(orbisAudioConf->channels[i])
            {
                if(orbisAudioConf->channels[i]->orbisaudiochannel_initialized==1)
                {
                    //scePthreadCancel(orbisAudioConf->channels[i]->threadHandle);
                    orbisAudioConf->channels[i]->threadHandle=0;

                    sceAudioOutClose(orbisAudioConf->channels[i]->audioHandle);
                    fprintf(1, "[orbisAudio] closing audio handle\n");
                    
                    orbisAudioConf->channels[i]->audioHandle=-1;
                    orbisAudioDestroyBuffersChannel(i);
                    fprintf(1, "[orbisAudio] free buffers channel\n");
                    
                    orbisAudioConf->channels[i]->orbisaudiochannel_initialized=-1;  
                }
                //free(orbisAudioConf->channels[i]);
            }
        }
        //free(orbisAudioConf);
        fprintf(1, "[orbisAudio] finished\n");
    }
}

int orbisAudioInit()
{
    //pthread_mutex_init(&wait_mutex, NULL);// = PTHREAD_MUTEX_INITIALIZER;

    int ret = sceAudioOutInit();
    if(ret<0)
    {
        fprintf(2, "[orbisAudio] sceAudioOutInit error 0x%08X\n",ret); return -1;
    }
    fprintf(1, "[orbisAudio] sceAudioOutInit return %d\n",ret);

    if(orbisAudioCreateConf() == 0)
    {
        fprintf(1, "[orbisAudio] initialized!\n");
        return orbisAudioConf->orbisaudio_initialized;
    }
    if (orbisAudioConf->orbisaudio_initialized == 1) 
    {
        fprintf(1, "[orbisAudio] is already initialized!\n");
        return orbisAudioConf->orbisaudio_initialized;
    }
    return -1;
}
