/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <string.h>
#include <stdlib.h>
#include "orbis2d.h"


Orbis2dTexture * orbis2dCreateEmptyTexture(unsigned int w, unsigned int h)
{
	
	Orbis2dTexture *img=NULL;
	img=malloc(sizeof(Orbis2dTexture));
	if(img!=NULL)
	{
		img->datap=malloc(w*h*4);
		if(img->datap==NULL)
		{
			free(img);
			return NULL;
		}
		img->width=w;
		img->height=h;
		img->depth=32;
	}
	return img;
}
void orbis2dDestroyTexture(Orbis2dTexture *texture)
{
	
	if(texture!=NULL)
	{
		if(texture->datap!=NULL)
		{
			free(texture->datap);
		}
	}
	free(texture);
}
uint32_t *orbis2dTextureGetDataPointer(Orbis2dTexture *texture)
{
	return texture->datap;
}
uint32_t orbis2dTextureGetStride(Orbis2dTexture *texture)
{
	return texture->width;
}
void orbis2dDrawTexture(Orbis2dTexture *texture, unsigned int x, unsigned int y)
{
	uint32_t *buf=NULL;
	if(texture!=NULL)
	{
		buf=orbis2dTextureGetDataPointer(texture);
		if(buf!=NULL)
		{
			orbis2dPutImage2(buf,x,y,texture->width,texture->height);
		}
	}
}
