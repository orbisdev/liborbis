/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/liborbis
 */
#include <stdint.h>
#include <stdbool.h>

#include <types/kernel.h>
#include <types/userservice.h>
#include <types/pad.h>



#define	ORBISPAD_L3		0x00000002
#define	ORBISPAD_R3		0x00000004
#define	ORBISPAD_OPTIONS	0x00000008
#define	ORBISPAD_UP		0x00000010
#define	ORBISPAD_RIGHT		0x00000020
#define	ORBISPAD_DOWN		0x00000040
#define	ORBISPAD_LEFT		0x00000080
#define	ORBISPAD_L2		0x00000100
#define	ORBISPAD_R2		0x00000200
#define	ORBISPAD_L1		0x00000400
#define	ORBISPAD_R1		0x00000800
#define	ORBISPAD_TRIANGLE	0x00001000
#define	ORBISPAD_CIRCLE		0x00002000
#define	ORBISPAD_CROSS		0x00004000
#define	ORBISPAD_SQUARE		0x00008000
#define	ORBISPAD_TOUCH_PAD	0x00100000
#define	ORBISPAD_INTERCEPTED	0x80000000


typedef struct OrbisPadConfig
{
	SceUserServiceUserId userId;
	ScePadData *padDataCurrent;
	ScePadData *padDataLast;
	unsigned int buttonsPressed;
	unsigned int buttonsReleased;
	unsigned int buttonsHold;
	int padHandle;
	int orbispad_initialized;
	
}OrbisPadConfig;

int orbisPadInit();
void orbisPadFinish();
OrbisPadConfig *orbisPadGetConf();
int orbisPadCreateConf();
int orbisPadSetConf(OrbisPadConfig *conf);
int orbisPadInitWithConf(OrbisPadConfig *conf);
bool orbisPadGetButtonHold(unsigned int filter);
bool orbisPadGetButtonPressed(unsigned int filter);
bool orbisPadGetButtonReleased(unsigned int filter);
unsigned int orbisPadGetCurrentButtonsPressed();
unsigned int orbisPadGetCurrentButtonsReleased();
void orbisPadSetCurrentButtonsPressed(unsigned int buttons);
void orbisPadSetCurrentButtonsReleased(unsigned int buttons);
int orbisPadUpdate();
