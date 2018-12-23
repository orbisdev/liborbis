/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/liborbis
 */
#include <stdint.h>

#include <types/kernel.h>


#ifdef __cplusplus
extern "C" {
#endif


#define ORBISAUDIO_ALIGN_SAMPLE(x,y) ((((x)+((y)-1))/(y))*(y))
#define ORBISAUDIO_MIN_LEN			256
#define ORBISAUDIO_MAX_LEN			2048
#define ORBISAUDIO_CHANNELS			5
#define ORBISAUDIO_CHANNEL_MAIN			0
#define ORBISAUDIO_CHANNEL_BGM			1
#define ORBISAUDIO_CHANNEL_VOICE		2
#define ORBISAUDIO_CHANNEL_PERSONAL		3
#define ORBISAUDIO_CHANNEL_AUX			4
#define ORBISAUDIO_NUM_BUFFERS			2
#define ORBISAUDIO_VOLUME_MAX			32768
#define ORBISAUDIO_VOLUME_FLAG_LEFT_CHANNEL	0
#define ORBISAUDIO_VOLUME_FLAG_RIGHT_CHANNEL	1
#define ORBISAUDIO_FORMAT_S16_MONO		0
#define ORBISAUDIO_FORMAT_S16_STEREO		1

typedef struct OrbisAudioStereoSample
{
  short l;
  short r;
} OrbisAudioStereoSample;

typedef struct OrbisAudioMonoSample
{
  short ch;
} OrbisAudioMonoSample;

typedef union OrbisAudioSample
{
  OrbisAudioStereoSample stereo;
  OrbisAudioMonoSample mono;
} OrbisAudioSample;

typedef void (*OrbisAudioCallback)(OrbisAudioSample *buffer,unsigned int samples,void *user_data);

typedef struct OrbisAudioChannel
{
	ScePthread threadHandle;
	int audioHandle;
	unsigned int leftVol;
	unsigned int rightVol;
	OrbisAudioCallback callback;
	void *userData;
	short *sampleBuffer[2];
	unsigned int samples[2];
	unsigned char paused;
	unsigned char stereo;
	unsigned int currentBuffer;
	int orbisaudiochannel_initialized;
}OrbisAudioChannel;

typedef struct OrbisAudioConfig
{
	OrbisAudioChannel *channels[ORBISAUDIO_CHANNELS];
	unsigned char orbisaudio_stop;
	int orbisaudio_initialized;
} OrbisAudioConfig;


int orbisAudioInit();
void orbisAudioFinish();
int orbisAudioGetStatus();
int orbisAudioGetChannelStatus(int chan);
int orbisAudioGetHandle(int chan);
int orbisAudioPlayBlock(unsigned int channel,unsigned int vol1,unsigned int vol2,void *buf);
int orbisAudioInitChannelWithoutCallback(unsigned int channel, unsigned int samples, unsigned int frequency, int format);
int orbisAudioInitChannel(unsigned int channel, unsigned int samples, unsigned int frequency, int format);
int orbisAudioPause(unsigned int channel);
int orbisAudioResume(unsigned int channel);
int orbisAudioStop();
int orbisAudioSetCallback(unsigned int channel,OrbisAudioCallback callback,void *userdata);
int orbisAudioInitWithConf(OrbisAudioConfig *conf);
OrbisAudioConfig *orbisAudioGetConf();

#ifdef __cplusplus
}
#endif