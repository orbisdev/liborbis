/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/liborbis
 */
#include <stdint.h>
#include <types/kernel.h>

#define ORBIS_IME_MODULE 0x0095

typedef enum OrbisKeyboardEventId{
	ORBIS_KEYBOARD_EVENT_OPEN	   			=	256,
	ORBIS_KEYBOARD_EVENT_KEYCODE_DOWN		=	257,
	ORBIS_KEYBOARD_EVENT_KEYCODE_UP			=	258,
	ORBIS_KEYBOARD_EVENT_KEYCODE_REPEAT		=	259,
	ORBIS_KEYBOARD_EVENT_CONNECTION			=	260,	
	ORBIS_KEYBOARD_EVENT_DISCONNECTION		=	261,
	ORBIS_KEYBOARD_EVENT_ABORT				=	262,
} OrbisKeyboardEventId;

typedef struct OrbisKeyboardConfig
{
	SceUserServiceUserId userId;
	int status;
	int caps_key;
	int control_key_left;
	int control_key_right;
	int shift_key_left;
	int shift_key_right;
	int alt_key_left;
	int alt_key_right;
	int cmd_key_left;
	int cmd_key_right;
	uint8_t key;
	uint8_t key_status;
	int orbiskeyboard_initialized;
	
}OrbisKeyboardConfig;


typedef struct
{
	SceUserServiceUserId userId;
	uint32_t resourceId[5];
} OrbisKeyboardResourceIdArray;

typedef struct
{
	uint16_t keycode;
	uint32_t status;//1.76 
	uint32_t unknown1;
	SceUserServiceUserId userId;
	uint32_t resourceId;
	uint32_t unknown2;
	uint64_t timestamp;	
} OrbisKeyboardKeycode;

typedef union
{
	int8_t notused[84];
	OrbisKeyboardKeycode keycode;							
	OrbisKeyboardResourceIdArray resourceIdArray;	
	void *candidateWord;
	int32_t candidateIndex;
	int32_t deviceType;
	uint32_t inputMethodState;
	int8_t reserved[64];
} OrbisKeyboardEventParam;

typedef struct OrbisKeyboardEvent {
	OrbisKeyboardEventId id;
	OrbisKeyboardEventParam param;
} OrbisKeyboardEvent;

typedef void (*OrbisKeyboardEventHandler)(
	void *arg,
	const OrbisKeyboardEvent *e
);
	
typedef struct OrbisKeyboardParam {
	uint32_t option;
	int8_t unknown1[4];
	void *arg;
	OrbisKeyboardEventHandler handler;
	int8_t unknown2[8];
} OrbisKeyboardParam;


int orbisKeyboardInit();
void orbisKeyboardFinish();
int orbisKeyboardOpen();
int orbisKeyboardRegisterUpdateHandler(void *arg,OrbisKeyboardEventHandler handler);
void orbisKeybordGetSpecialKeys(const OrbisKeyboardEvent *e);
uint8_t orbisKeyboardGetMsxKey();
uint8_t orbisKeyboardGetMsxKeyShift();
uint8_t orbisKeyboardGetMsxKeyCtrl();
uint8_t orbisKeyboardGetMsxKeyStatus();
void orbisKeyboardUpdate();
int sceImeUpdate(OrbisKeyboardEventHandler handler);
int sceImeKeyboardOpen(SceUserServiceUserId userId,const OrbisKeyboardParam *param);
int sceImeKeyboardClose(SceUserServiceUserId userId);


