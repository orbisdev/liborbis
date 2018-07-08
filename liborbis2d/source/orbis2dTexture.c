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

/* tx and ty are the coordinates in the texture, usefull if you want draw just a piece of texture, if not interested in that just set both to 0
 * tw and th are the dimensions of texture, if you set them to 0 the whole image will be draw */

void orbis2dDrawTexture(Orbis2dTexture *texture, uint32_t x, uint32_t y, uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th)
{
	uint32_t *buf=NULL;
	if(texture!=NULL)
	{
		buf=orbis2dTextureGetDataPointer(texture);
		if(buf!=NULL)
		{
			if(tw==0 && th==0) {tw=texture->width; th=texture->height;}
			if(tx+tw > texture->width) tw = (texture->width - tx);
			if(ty+th > texture->height) th = (texture->height - ty);
			orbis2dPutImage2(buf,x,y,tx,ty,tw,th,texture->width);
		}
	}
}
