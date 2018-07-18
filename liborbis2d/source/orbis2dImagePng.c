/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <string.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <ps4link.h>
#include <png.h>
#include "orbis2d.h"

#define PNG_SIGSIZE (8)

static void orbis2dReadPngFromHost(png_structp png_ptr, png_bytep data, png_size_t length)
{
	int fd = *(int *)png_get_io_ptr(png_ptr);
	ps4LinkRead(fd,data,length);
}


static void orbis2dReadPngFromBuffer(png_structp png_ptr, png_bytep data, png_size_t length)
{
	uint64_t *address = png_get_io_ptr(png_ptr);
	memcpy(data, (void *)*address, length);
	*address += length;
}

static Orbis2dTexture *orbis2dLoadPngGeneric(const void *io_ptr,png_rw_ptr read_data_fn)
{
	png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if (png_ptr==NULL)
	{
		goto error_create_read;
	}

	png_infop info_ptr=png_create_info_struct(png_ptr);
	if (info_ptr==NULL)
	{
		goto error_create_info;
	}

	png_bytep *row_ptrs=NULL;

	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
		if (row_ptrs!=NULL)
		{
			free(row_ptrs);
		}
		return NULL;
	}

	png_set_read_fn(png_ptr,(png_voidp)io_ptr,read_data_fn);
	png_set_sig_bytes(png_ptr,PNG_SIGSIZE);
	png_read_info(png_ptr,info_ptr);

	unsigned int width, height;
	int bit_depth, color_type;

	png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,NULL,NULL,NULL);

	if ((color_type==PNG_COLOR_TYPE_PALETTE && bit_depth<=8)
		|| (color_type==PNG_COLOR_TYPE_GRAY && bit_depth<8)
		|| png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS)
		|| (bit_depth==16))
	{
		png_set_expand(png_ptr);
	}

	if (bit_depth == 16)
		png_set_scale_16(png_ptr);

	if (bit_depth==8 && color_type==PNG_COLOR_TYPE_RGB)
		png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);

	if (color_type==PNG_COLOR_TYPE_GRAY ||
	    color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	if (color_type==PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);
	}

	if (color_type==PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth<8)
		png_set_packing(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	row_ptrs = (png_bytep *)malloc(sizeof(png_bytep)*height);
	if (!row_ptrs)
		goto error_alloc_rows;

	Orbis2dTexture *texture = orbis2dCreateEmptyTexture(width,height);
	if (!texture)
		goto error_create_tex;

	uint32_t *texture_data=orbis2dTextureGetDataPointer(texture);
	unsigned int stride=orbis2dTextureGetStride(texture);

	int i;
	for (i=0;i<height;i++)
	{
		row_ptrs[i]=(png_bytep)(texture_data+i*stride);
	}

	png_read_image(png_ptr, row_ptrs);

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
	free(row_ptrs);

	return texture;

error_create_tex:
	free(row_ptrs);
error_alloc_rows:
	png_destroy_info_struct(png_ptr,&info_ptr);
error_create_info:
	png_destroy_read_struct(&png_ptr,(png_infopp)0,(png_infopp)0);
error_create_read:
	return NULL;
}


Orbis2dTexture *orbis2dLoadPngFromHost(const char *filename)
{
	png_byte pngsig[PNG_SIGSIZE];
	int fd;

	if((fd = ps4LinkOpen(filename, O_RDONLY, 0777)) < 0) 
	{
		goto exit_error;
	}

	if (ps4LinkRead(fd,pngsig,PNG_SIGSIZE)!=PNG_SIGSIZE) 
	{
		goto exit_close;
	}

	if (png_sig_cmp(pngsig,0,PNG_SIGSIZE)!=0) 
	{
		goto exit_close;
	}

	Orbis2dTexture *texture=orbis2dLoadPngGeneric((void *)&fd,orbis2dReadPngFromHost);
	ps4LinkClose(fd);
	return texture;

exit_close:
	ps4LinkClose(fd);
exit_error:
	return NULL;
}

Orbis2dTexture *orbis2dLoadPngFromHost_v2(const char *path)
{
	int fd;             // descriptor to manage file from host0
	int filesize;       // variable to control file size
	uint8_t *buf=NULL;  // buffer for read from host0 file

	// we open file in read only from host0 ps4sh include the full path with host0:/.......
	fd=ps4LinkOpen(path,O_RDONLY,0);

	if(fd<0)  //If we can't open file from host0 print  the error and return
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkOpen returned error %d\n",fd);
		return NULL;
	}
	filesize=ps4LinkLseek(fd,0,SEEK_END);  // Seek to final to get file size
	if(filesize<0)                         // If we get an error print it and return
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkSeek returned error %d\n",fd);
		ps4LinkClose(fd);
		return NULL;
	}

	ps4LinkLseek(fd,0,SEEK_SET);  //Seek back to start
	buf=malloc(filesize);         //Reserve  memory for read buffer

	int numread=ps4LinkRead(fd,buf,filesize);  //Read filsesize bytes to buf
	ps4LinkClose(fd);  //Close file

	if(numread!=filesize)                      //if we don't get filesize bytes we are in trouble
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkRead returned error %d\n",numread);
		return NULL;
	}

	Orbis2dTexture *texture=orbis2dLoadPngFromBuffer(buf);  //create png from buf

	return texture;
}

Orbis2dTexture *orbis2dLoadPngFromBuffer(const void *buffer)
{
	if(png_sig_cmp((png_byte *)buffer,0,PNG_SIGSIZE)!=0) 
	{
		return NULL;
	}
	uint64_t buffer_address=(uint64_t)buffer+PNG_SIGSIZE;

	return orbis2dLoadPngGeneric((void *)&buffer_address,orbis2dReadPngFromBuffer);
}
