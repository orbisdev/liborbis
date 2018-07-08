/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <kernel.h>
#include <types/event.h>


#include <gnmdriver.h>
#include <systemservice.h>
#include <videoout.h>
#include <types/userservice.h>
#include <debugnet.h>
#include "orbis2d.h"

Orbis2dConfig *orbconf=NULL;
int orbis2d_external_conf=-1;
int64_t flipArgCounter=0; 

void orbis2dFinish()
{
	int ret;
	if(orbis2d_external_conf!=1)
	{
		if(orbconf->orbis2d_initialized==1)
		{		
			ret=sceVideoOutClose(orbconf->videoHandle);
		
			debugNetPrintf(DEBUG,"liborbis2d sceVideoOutClose return 0x%8x\n",ret);
		}
		orbconf->orbis2d_initialized=-1;
		debugNetPrintf(DEBUG,"liborbis2d finished\n");
	}
}

Orbis2dConfig *orbis2dGetConf()
{
	if(orbconf)
	{
		return orbconf;
	}
	
	return NULL; 
}
int orbis2dCreateConf()
{	
	if(!orbconf)
	{
		orbconf=(Orbis2dConfig *)malloc(sizeof(Orbis2dConfig));
		
		orbconf->bgColor=0x80ffffff; //white
		orbconf->videoMemStackAddr=0;
		orbconf->videoMemStackSize=1024*1024* 192;
		orbconf->videoMemStackTopAddr=0;
		orbconf->videoMemStackBaseAddr=0;
		orbconf->videoMemOffset=0;
		orbconf->width=ATTR_WIDTH;
		orbconf->pitch=ATTR_WIDTH;
		orbconf->height=ATTR_HEIGHT;
		orbconf->pixelFormat=0x80000000;
		orbconf->bytesPerPixel=4;
		orbconf->tilingMode=ORBIS2D_MODE_LINEAR;
		orbconf->flipMode=ORBIS2D_FLIP_MODE_VSYNC;
		orbconf->flipRate=ORBIS2D_FLIP_RATE;
		orbconf->videoHandle=-1;
		orbconf->currentBuffer=0;
		orbconf->orbis2d_initialized=-1;
		
		return 0;
	}
	
	if(orbconf->orbis2d_initialized==1)
	{
		return orbconf->orbis2d_initialized;
	}
	//something weird happened
	return -1;			
}
int orbis2dSetConf(Orbis2dConfig *conf)
{
	if(conf)
	{
		orbconf=conf;
		orbis2d_external_conf=1;
		return orbconf->orbis2d_initialized;
	}
	
	return 0; 
}
int orbis2dInitWithConf(Orbis2dConfig *conf)
{
	int ret;
	ret=orbis2dSetConf(conf);
	if(ret)
	{
		debugNetPrintf(DEBUG,"liborbis2d already initialized using configuration external\n");
		debugNetPrintf(DEBUG,"orbis2d_initialized=%d\n",orbconf->orbis2d_initialized);
		debugNetPrintf(DEBUG,"ready to have a lot of fun...\n");
		return orbconf->orbis2d_initialized;
	}
	else
	{
		return 0;
	}
}

int orbis2dWaitFlipArg(SceKernelEqueue *flipQueue)
{
	int ret;
	int event_out;
	SceKernelEvent event;
	
	
	while(1)
	{
		SceVideoOutFlipStatus status;
		ret=sceVideoOutGetFlipStatus(orbconf->videoHandle, &status);
		if(ret>=0)
		{
			
			if(status.flipArg>=(orbconf->flipArgLog[orbconf->currentBuffer] +1))
			{
				return 0;
			}

			ret=sceKernelWaitEqueue(*flipQueue, &event, 1, &event_out, 0);
			if(ret>=0)
			{
				//debugNetPrintf(DEBUG,"liborbis2d sceKernelWaitEqueue return %d\n",ret);
				
			}
			else
			{
				debugNetPrintf(DEBUG,"liborbis2d sceKernelWaitEqueue return error 0x%8x\n",ret);
			}
		}
		else
		{
			debugNetPrintf(DEBUG,"liborbis2d sceVideoOutGetFlipStatus return error 0x%8x\n",ret);
				
		}

	}


	return 0;

}
void orbis2dFinishDrawing(int64_t flipArg)
{
	int ret;

	sceGnmFlushGarlic();
		
	// request flip to the buffer
	ret=sceVideoOutSubmitFlip(orbconf->videoHandle, orbconf->currentBuffer, orbconf->flipMode, flipArg);
	
	orbconf->flipArgLog[orbconf->currentBuffer]=flipArg;

}
void orbis2dStartDrawing()
{

	orbis2dWaitFlipArg(&orbconf->flipQueue);

}

void orbis2dDrawPixelColor(int x, int y, uint32_t pixelColor)
{
	int color;
	int pixel = (y * orbconf->pitch) + x;

	color=pixelColor;

	
	((uint32_t *)orbconf->surfaceAddr[orbconf->currentBuffer])[pixel]=color;

}

void orbis2dDrawLineColor(uint32_t x, uint32_t y, uint32_t x2, uint32_t y2, uint32_t pixelColor)
{
	int32_t i = 0, dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	int32_t w = x2 - x;
	int32_t h = y2 - y;
	if(w < 0) dx1 = -1; else if(w > 0) dx1 = 1;
	if(h < 0) dy1 = -1; else if(h > 0) dy1 = 1;
	if(w < 0) dx2 = -1; else if(w > 0) dx2 = 1;
	int32_t l = abs(w);
	int32_t s = abs(h);
	if(!(l > s))
	{
		l = abs(h);
		s = abs(w);
	if(h < 0) dy2 = -1; else if(h > 0) dy2 = 1;
		dx2 = 0;
	}
	int32_t num = l >> 1;
	for(i = 0; i <= l; i++)
	{
		orbis2dDrawPixelColor(x, y, pixelColor);
		num+=s;
		if(!(num < l))
		{
			num-=l;
			x+=dx1;
			y+=dy1;
		}
		else
		{
			x+=dx2;
			y+=dy2;
		}
	}
}

/* circle helper function */
static void circle_points(int32_t x_c, int32_t y_c, int32_t x, int32_t y, uint32_t pixelColor)
{
	orbis2dDrawPixelColor(x_c + x, y_c + y, pixelColor);
	orbis2dDrawPixelColor(x_c - x, y_c + y, pixelColor);
	orbis2dDrawPixelColor(x_c + x, y_c - y, pixelColor);
	orbis2dDrawPixelColor(x_c - x, y_c - y, pixelColor);
	orbis2dDrawPixelColor(x_c + y, y_c + x, pixelColor);
	orbis2dDrawPixelColor(x_c - y, y_c + x, pixelColor);
	orbis2dDrawPixelColor(x_c + y, y_c - x, pixelColor);
	orbis2dDrawPixelColor(x_c - y, y_c - x, pixelColor);
}

/* circle helper function */
static void circle_lines(int32_t x_c, int32_t y_c, int32_t x, int32_t y, uint32_t pixelColor)
{
	orbis2dDrawLineColor(x_c - x, y_c + y, x_c + x, y_c + y, pixelColor);
	orbis2dDrawLineColor(x_c - x, y_c - y, x_c + x, y_c - y, pixelColor);
	orbis2dDrawLineColor(x_c - y, y_c + x, x_c + y, y_c + x, pixelColor);
	orbis2dDrawLineColor(x_c - y, y_c - x, x_c + y, y_c - x, pixelColor);
}

void orbis2dDrawCircleColor(int32_t x_c, int32_t y_c, int32_t r, unsigned char filled, uint32_t pixelColor)
{
	int32_t x = 0;
	int32_t y = r;
	int32_t p = 1 - r;

	if(filled)
		circle_lines(x_c, y_c, x, y, pixelColor);
	else
		circle_points(x_c, y_c, x, y, pixelColor);

	while(x < y)
	{
		x++;
		if(p < 0)
		{
			p += 2 * x + 1;
		}
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		if(filled)
			circle_lines(x_c, y_c, x, y, pixelColor);
		else
			circle_points(x_c, y_c, x, y, pixelColor);
	}
}

void orbis2dPutImage(uint32_t *buf,int x, int y, int w, int h)
{
	int x0, y0;
	unsigned int R,G,B;
	for(y0=0;y0<h-y;y0++) 
	{
		for(x0=0;x0<w-x;x0++) 
		{
			
			//R=buf[y0*w+x0]&0xFF0000;
			//G=buf[y0*w+x0]&0x00FF00;
			//B=buf[y0*w+x0]&0x0000FF;
			orbis2dDrawPixelColor(x0,y0,0x80<<24|buf[y0*w+x0]);
		}
	}
}
void orbis2dPutImage2(uint32_t *buf,int x, int y, int w, int h)
{
	int x0, y0;
	unsigned int R,G,B,A;
	for(y0=0;y0<h;y0++) 
	{
		for(x0=0;x0<w;x0++) 
		{
			A=(buf[y0*w+x0]&0xFF000000)>>24;
			B=(buf[y0*w+x0]&0xFF0000)>>16;
			G=(buf[y0*w+x0]&0x00FF00)>>8;
			R=(buf[y0*w+x0]&0x0000FF);
			orbis2dDrawPixelColor(x0+x,y0+y,A<<24|R<<16|G<<8|B);
		}
	}
}
void orbis2dPutImage3(uint32_t *buf,int x, int y, int w, int h)
{
	int x0, y0;
	unsigned int R,G,B,A;
	for(y0=0;y0<h-y;y0++) 
	{
		for(x0=0;x0<w-x;x0++) 
		{
			A=(buf[y0*w+x0]&0xFF000000)>>24;
			B=(buf[y0*w+x0]&0xFF0000)>>16;
			G=(buf[y0*w+x0]&0x00FF00)>>8;
			R=(buf[y0*w+x0]&0x0000FF);
			orbis2dDrawPixelColor(x0,y0,A<<24|B<<16|G<<8|R);
		}
	}
}
void orbis2dPutImage4(uint32_t *buf,int x, int y, int w, int h)
{
	int x0, y0;
	unsigned int R,G,B,A;
	for(y0=0;y0<h-y;y0++) 
	{
		for(x0=0;x0<w-x;x0++) 
		{
			A=(buf[y0*w+x0]&0xFF000000)>>24;
			B=(buf[y0*w+x0]&0xFF0000)>>16;
			G=(buf[y0*w+x0]&0x00FF00)>>8;
			R=(buf[y0*w+x0]&0x0000FF);
			orbis2dDrawPixelColor(x0,y0,0x80<<24|B<<16|G<<8|R);
		}
	}
}
void orbis2dPutImage5(uint32_t *buf,int x, int y, int w, int h)
{
	int x0, y0;
	unsigned int R,G,B,A;
	for(y0=0;y0<h-y;y0++) 
	{
		for(x0=0;x0<w-x;x0++) 
		{
			A=(buf[y0*w+x0]&0xFF000000)>>24;
			B=(buf[y0*w+x0]&0xFF0000)>>16;
			G=(buf[y0*w+x0]&0x00FF00)>>8;
			R=(buf[y0*w+x0]&0x0000FF);
			orbis2dDrawPixelColor(x0,y0,0x80<<24|R<<16|G<<8|B);
		}
	}
}
void orbis2dDrawRectColor(int x, int w, int y, int h, uint32_t color)
{
	int x0, y0;
	for(y0=y;y0<y+h;y0++) 
	{
		for(x0=x;x0<x+w;x0++) 
		{
			orbis2dDrawPixelColor(x0,y0,color);
		}
	}
}
void orbis2dClearBuffer()
{
	orbis2dDrawRectColor(0, orbconf->width, 0, orbconf->height, orbconf->bgColor);
}
void orbis2dSwapBuffers()
{
	orbconf->currentBuffer=(orbconf->currentBuffer+1)%ORBIS2D_DISPLAY_BUFFER_NUM;
	//debugNetPrintf(DEBUG,"liborbis2d currentBuffer  %d\n",orbconf->currentBuffer);
	
}
void *orbis2dMalloc(int size)
{
	uint64_t offset=orbconf->videoMemStackAddr;

	if((orbconf->videoMemStackAddr+size)>orbconf->videoMemStackTopAddr)
	{
		debugNetPrintf(DEBUG,"orbis2dMalloc videoMemStackAddr 0x%lx greater than videoMemStackTopAddr 0x%lx \n",orbconf->videoMemStackAddr,orbconf->videoMemStackTopAddr);
		
		return NULL;
	}
	orbconf->videoMemStackAddr+=size;
	
	return (void *)(offset);
}
void orbis2dAllocDisplayBuffer(int displayBufNum)
{
	int i;

	int bufSize=orbconf->pitch*orbconf->height*orbconf->bytesPerPixel;
	for (i=0;i<displayBufNum;i++) 
	{
		orbconf->surfaceAddr[i]= orbis2dMalloc(bufSize);
		debugNetPrintf(DEBUG,"liborbis2d orbis2dMalloc buffer %d, new pointer %p \n",i,	orbconf->surfaceAddr[i]);
		
	}
	debugNetPrintf(DEBUG,"liborbis2d orbis2dAllocDisplayBuffer done\n");
}
int orbis2dInitDisplayBuffer(int num, int bufIndexStart)
{
	SceVideoOutBufferAttribute attr;
	int ret;
	sceVideoOutSetBufferAttribute(&attr,orbconf->pixelFormat,orbconf->tilingMode,0,orbconf->width,orbconf->height,orbconf->pitch);

	debugNetPrintf(DEBUG,"liborbis2d sceVideoOutSetBufferAttribute done\n");
	
	ret=sceVideoOutRegisterBuffers(orbconf->videoHandle, bufIndexStart, orbconf->surfaceAddr, num, &attr);

	debugNetPrintf(DEBUG,"liborbis2d sceVideoOutRegisterBuffers return 0x%8x\n",ret);

	return ret;
}
int orbis2dInitMemory()
{
	int ret;
	off_t start;

	const uint32_t align=2*1024*1024;

	ret=sceKernelAllocateDirectMemory(0,sceKernelGetDirectMemorySize(),orbconf->videoMemStackSize,align,3,&start);
	if(ret==0)
	{

		orbconf->videoMemOffset=start;

		void* pointer=NULL;
		
		ret=sceKernelMapDirectMemory(&pointer,orbconf->videoMemStackSize,0x33,0,start,align);
		if(ret==0)
		{
			orbconf->videoMemStackBaseAddr=(uintptr_t)pointer;
			orbconf->videoMemStackTopAddr=orbconf->videoMemStackBaseAddr+orbconf->videoMemStackSize;
			orbconf->videoMemStackAddr=orbconf->videoMemStackBaseAddr;
		}
	}
	return ret;

}
int orbis2dInitVideoHandle()
{
	int handle;
	int ret;

	handle=sceVideoOutOpen(0xff, 0, 0, NULL);
	
	if(handle<0)
	{
		debugNetPrintf(DEBUG,"liborbis2d sceVideoOutOpen return error 0x%8x\n",handle);

	}
	else
	{
		debugNetPrintf(DEBUG,"liborbis2d sceVideoOutOpen return video handle 0x%8x\n",handle);

		if(handle>0)
		{
			ret=sceKernelCreateEqueue(&orbconf->flipQueue,"queue to wait flip");
			if(ret>=0)
			{
				debugNetPrintf(DEBUG,"sceKernelCreateEqueue return %d\n",ret);
				
				ret=sceVideoOutAddFlipEvent(orbconf->flipQueue, handle, NULL);
				if(ret<0)
				{
					debugNetPrintf(DEBUG,"liborbis2d sceVideoOutAddFlipEvent return 0x%8x\n",ret);
					ret=sceVideoOutClose(handle);
					debugNetPrintf(DEBUG,"liborbis2d sceVideoOutClose return %d\n",ret);
					handle=-1;
				}
				else
				{
					debugNetPrintf(DEBUG,"liborbis2d sceVideoOutAddFlipEvent return %d\n",ret);		
				}
			}
			else
			{
				debugNetPrintf(DEBUG,"sceKernelCreateEqueue return error 0x%8x\n",ret);
				ret=sceVideoOutClose(handle);
				debugNetPrintf(DEBUG,"liborbis2d sceVideoOutClose return %d\n",ret);
				
				handle=-1;
			}
		}	
	}		
	return handle;
}
int orbis2dInit()
{
	int ret;
	int bufIndex;

	if(orbis2dCreateConf()==1)
	{
			return orbconf->orbis2d_initialized;
	}
	if (orbconf->orbis2d_initialized==1) 
	{
		debugNetPrintf(DEBUG,"liborbis2d is already initialized!\n");
		return orbconf->orbis2d_initialized;
	}
	//Get video handle
	orbconf->videoHandle=orbis2dInitVideoHandle();


	if(orbconf->videoHandle>0)
	{
		//init memory
		ret=orbis2dInitMemory();

		if(ret==0)
		{	
			debugNetPrintf(DEBUG,"liborbis2d video memory initialized\n");

			//init memory for display buffers
			orbis2dAllocDisplayBuffer(ORBIS2D_DISPLAY_BUFFER_NUM);
			
			// register display buffers
			orbis2dInitDisplayBuffer(ORBIS2D_DISPLAY_BUFFER_NUM,0);

			// set status of each buffer with flipArg
			for(bufIndex=0;bufIndex<ORBIS2D_DISPLAY_BUFFER_NUM;bufIndex++) 
			{
				orbconf->flipArgLog[bufIndex]= -2; 
			}

			// prepare initial clear color to the display buffers
			for (bufIndex=0;bufIndex<ORBIS2D_DISPLAY_BUFFER_NUM;bufIndex++) 
			{
				orbis2dClearBuffer();
				orbis2dSwapBuffers();
			}
			
			ret=sceVideoOutSetFlipRate(orbconf->videoHandle,ORBIS2D_FLIP_RATE);
		}
		
		
		
		orbconf->orbis2d_initialized=1;
		debugNetPrintf(DEBUG,"liborbis2d initialized\n");
		
	}
	return orbconf->orbis2d_initialized;
}

