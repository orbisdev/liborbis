/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/liborbis
 */
#include <stdint.h>

#include <types/kernel.h>
#include <types/videoout.h>


#define ORBIS2D_DISPLAY_BUFFER_NUM 2 
#define ORBIS2D_FLIP_RATE 0 
#define ORBIS2D_FLIP_MODE_VSYNC 1
#define ORBIS2D_FLIP_MODE_HSYNC 2
#define ORBIS2D_FLIP_MODE_WINDOW 3
#define ORBIS2D_MODE_TILE 0
#define ORBIS2D_MODE_LINEAR 1


#define ATTR_WIDTH 1280 
#define ATTR_HEIGHT 720




typedef struct Orbis2dConfig
{
	uint32_t bgColor;
	uint64_t videoMemStackAddr;
	uint32_t videoMemStackSize;
	uint64_t videoMemStackTopAddr;
	uint64_t videoMemStackBaseAddr;
	off_t videoMemOffset;
	int width;
	int pitch; 
	int height;
	int pixelFormat;
	int bytesPerPixel;
	int tilingMode;
	void *surfaceAddr[ORBIS2D_DISPLAY_BUFFER_NUM];
	int64_t flipArgLog[ORBIS2D_DISPLAY_BUFFER_NUM];
	int flipMode;
	int flipRate;
	SceKernelEqueue flipQueue;
	int videoHandle;
	int currentBuffer;
	int orbis2d_initialized;
	
}Orbis2dConfig;

typedef struct Orbis2dTexture
{
	uint32_t *datap;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
}Orbis2dTexture;

int orbis2dInit();
void orbis2dFinish();
int orbis2dInitWithConf(Orbis2dConfig *conf);
int orbis2dSetConf(Orbis2dConfig *conf);
int orbis2dCreateConf();
Orbis2dConfig *orbis2dGetConf();
void orbis2dSwapBuffers();
void orbis2dClearBuffer();
void orbis2dDrawRectColor(int x, int w, int y, int h, uint32_t color);
void orbis2dDrawPixelColor(int x, int y, uint32_t pixelColor);
void orbis2dDrawPixelColor_WAlpha(int x, int y, uint32_t pixelColor);
void orbis2dDrawLineColor(uint32_t x, uint32_t y, uint32_t x2, uint32_t y2, uint32_t pixelColor);
void orbis2dDrawCircleColor(int32_t x_c, int32_t y_c, int32_t r, unsigned char filled, uint32_t pixelColor);
void orbis2dStartDrawing();
void orbis2dFinishDrawing(int64_t flipArg);
void orbis2dPutImage(uint32_t *buf,int x, int y, int w, int h);
void orbis2dPutImage2(uint32_t *buf,int x, int y, int w, int h);
void orbis2dPutImage3(uint32_t *buf,int x, int y, int w, int h);
void orbis2dPutImage4(uint32_t *buf,int x, int y, int w, int h);
void orbis2dPutImage5(uint32_t *buf,int x, int y, int w, int h);
Orbis2dTexture * orbis2dCreateEmptyTexture(unsigned int w, unsigned int h);
void orbis2dDestroyTexture(Orbis2dTexture *texture);
uint32_t *orbis2dTextureGetDataPointer(Orbis2dTexture *texture);
uint32_t orbis2dTextureGetStride(Orbis2dTexture *texture);
Orbis2dTexture *orbis2dLoadPngFromBuffer(const void *buffer);
Orbis2dTexture *orbis2dLoadPngFromHost(const char *filename);
void orbis2dDrawTexture(Orbis2dTexture *texture,unsigned int x, unsigned int y);
