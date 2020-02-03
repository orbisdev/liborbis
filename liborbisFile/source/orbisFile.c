/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <kernel.h>
#include <ps4link.h>
#include <debugnet.h>
#include <sys/fcntl.h>
#include <string.h>
#include <stdlib.h>

#define HOST0 "host0:"
#define ORBISFILE_MAX_OPEN_DESCRIPTORS 10
int orbisFileRemoteOpenDescriptors[ORBISFILE_MAX_OPEN_DESCRIPTORS];
int orbisfile_initialized=0;
int lastopenedslot=-1;

int orbisCheckSlotByFd(int fd)
{
	if(fd<0) return -1;

	int i, slot = -1;
	// search slot
	for(i=0;i<ORBISFILE_MAX_OPEN_DESCRIPTORS;i++) 
	{ 
		if(orbisFileRemoteOpenDescriptors[i]==fd) return i;
	}
	return slot;
}
int orbisCheckFreeRemoteDescriptor()
{
	int i, slot=-1;
	// search slot
	for(i=0;i<ORBISFILE_MAX_OPEN_DESCRIPTORS;i++) 
	{ 
		if(orbisFileRemoteOpenDescriptors[i]==-1) return i;
	}
	return slot;
}
int orbisOpen(const char *file, int flags, int mode) 
{
	if(!file) return 0x80020016;

	int ret, slot;

	if (strncmp(file,HOST0,sizeof(HOST0)-1)==0) 
	{
		slot=orbisCheckFreeRemoteDescriptor();
		if(slot != lastopenedslot)
			debugNetPrintf(INFO,"orbisCheckFreeRemoteDescriptor = %d\n",slot);

		if(slot>=0)
		{
			lastopenedslot = slot;
			ret=ps4LinkOpen(file,flags,mode);
			//debugNetPrintf(DEBUG,"[ORBISFILE] ps4LinkOpen, slot=%d ret=%d, lastopenedslot:%d\n",slot, ret, lastopenedslot); // 0 11
			if(ret>0)
			{
				orbisFileRemoteOpenDescriptors[slot]=ret;
			}
			else
			{
				debugNetPrintf(ERROR,"[ORBISFILE] slot=%d fd=%d\n",slot,ret);
			}
			return ret;
		}
		debugNetPrintf(DEBUG,"[ORBISFILE] no free slot available\n");
		return 0x80020016;
	}
	else
	{
		return sceKernelOpen(file,flags,mode);
	}
}
int orbisClose(int fd)
{
	if(!fd) return 0x80020009;

	int ret,
		slot=orbisCheckSlotByFd(fd);
	//debugNetPrintf(ERROR,"orbisCheckSlotByFd(%d) = %d\n", fd, slot); // 11 0
	if(slot>=0)
	{
		ret=ps4LinkClose(fd);
		if(ret)
			debugNetPrintf(DEBUG,"[ORBISFILE] orbisClose slot=%d fd=%d ret=%d\n",slot,fd,ret);
		else
			orbisFileRemoteOpenDescriptors[slot]=-1;

		return ret;
	}
	return sceKernelClose(fd);
}
int orbisRead(int fd, void *data, size_t size)
{
	if(!fd)	  return 0x80020009;
	if(!data) return 0x8002000e;

	int ret,
		slot=orbisCheckSlotByFd(fd);
	if(slot>=0)
	{
		ret=ps4LinkRead(fd,data,size);
		if(ret != size)
			debugNetPrintf(DEBUG,"[ORBISFILE] orbisRead slot=%d fd=%d byteread=%d\n",slot,fd,ret);
		
		return ret;
	}
	return sceKernelRead(fd,data,size);
}
int orbisWrite(int fd, const void *data, size_t size)
{
	if(!fd)	  return 0x80020009;
	if(!data) return 0x8002000e;

	int ret,
		slot=orbisCheckSlotByFd(fd);
	if(slot>=0)
	{
		ret=ps4LinkWrite(fd,data,size);
		if(ret != size)
			debugNetPrintf(DEBUG,"[ORBISFILE] orbisWrite slot=%d fd=%d bytewrite=%d\n",slot,fd,ret);
		
		return ret;
	}
	return sceKernelWrite(fd,data,size);
}
int orbisLseek(int fd, int offset, int whence)
{
	if(!fd)	return 0x80020009;

	int ret,
		slot=orbisCheckSlotByFd(fd);
	if(slot>=0)
	{
		//debugNetPrintf(DEBUG,"[ORBISFILE] offset=%d,whence=%d\n",offset,whence);
		ret=ps4LinkLseek(fd,offset,whence);
		if(ret<0)
			debugNetPrintf(DEBUG,"[ORBISFILE] orbisLseek slot=%d fd=%d ret=%d\n",slot,fd,ret);
		
		return ret;
	}
	return sceKernelLseek(fd,offset,whence);
}
int orbisDopen(const char *path)
{
	if(!path) return 0x80020016;

	int ret, slot;

	if (strncmp(path,HOST0,sizeof(HOST0)-1)==0) 
	{
		slot=orbisCheckFreeRemoteDescriptor();
		if(slot>=0)
		{
			ret=ps4LinkDopen(path);
			if(ret>0)
				orbisFileRemoteOpenDescriptors[slot]=ret;

			debugNetPrintf(DEBUG,"[ORBISFILE] orbisDopen slot=%d dfd=%d\n",slot,ret);
			return ret;
		}
		debugNetPrintf(DEBUG,"[ORBISFILE] orbisOpen no free slot available\n");
		return 0x80020016;
	}
	else
	{
		return open(path, O_RDONLY, 0);
	}
	
}
int orbisDclose(int dfd)
{
	int ret=0;
	int slot=-1;
	if(!dfd)
	{
		return 0x80020009;
	}
	slot=orbisCheckSlotByFd(dfd);
	if(slot>=0)
	{
		ret=ps4LinkDclose(dfd);
		debugNetPrintf(DEBUG,"[ORBISFILE] orbisClose slot=%d dfd=%d \n",slot,dfd);
		orbisFileRemoteOpenDescriptors[slot]=-1;
		return ret;
	}
	return sceKernelClose(dfd);
}
int orbisDread(int dfd, struct dirent *entry)
{
	int ret;
	int slot=-1;
	if(!dfd)
	{
		return 0x80020009;
	}
	if(!entry)
	{
		return 0x8002000e;
	}
	slot=orbisCheckSlotByFd(dfd);
	if(slot>=0)
	{	
		OrbisDirEntry *dir;
		//memset(&ent,0,sizeof(OrbisFileEntry));
		dir=malloc(sizeof(OrbisDirEntry));
		ret=ps4LinkDread(dfd,dir);
		if(ret>0)
		{
			//entry->d_fileno=-1;
			//entry->d_reclen=;
			entry->d_type=dir->type;
			entry->d_namlen=dir->namelen;
			memset(entry->d_name,0,strlen(entry->d_name));
			strncpy(entry->d_name,dir->name,dir->namelen);
		}
		debugNetPrintf(DEBUG,"[ORBISFILE] orbisDread slot=%d dfd=%d ret=%d\n",slot,dfd,ret);
		
		return ret;
	}
	return getdents(dfd,(char *)entry,sizeof(struct dirent));
	
}
int orbisMkdir(const char* path, int mode)
{
	int ret=0;
	if(!path)
	{
		return 0x80020016;
	}
	if (strncmp(path,HOST0,sizeof(HOST0)-1)==0) 
	{
		ret=ps4LinkMkdir(path,mode);
		debugNetPrintf(DEBUG,"[ORBISFILE] orbisMkdir path=%s ret=%d\n",path,ret);
		return ret;
	}
	else
	{
		return sceKernelMkdir(path,mode);
	}
}

/*
 each orbisFileGetFileContent() call will update this
 variable letting know filesize on succesful exit.
*/
size_t _orbisFile_lastopenFile_size;

unsigned char * orbisFileGetFileContent(const char *filename)
{
	_orbisFile_lastopenFile_size = -1;  // reset exported variable

	int pFile=orbisOpen(filename,O_RDONLY,0);

	if(pFile<=0)
	{
		debugNetPrintf(DEBUG,"[ORBISFILE] Failed to read file %s\n", filename);
		return 0;
	}

	// obtain file size:
	int32_t fileSize=orbisLseek(pFile,0,SEEK_END);
	orbisLseek(pFile,0,SEEK_SET);  // Seek back to start
	if(fileSize<0)
	{
		debugNetPrintf(DEBUG,"[ORBISFILE] Failed to read size of file %s\n", filename);
		orbisClose(pFile);
		return NULL;
	}

	unsigned char *pText=malloc(sizeof(char)*fileSize+1);
	if(!pText)
	{
		debugNetPrintf(DEBUG,"[ORBISFILE] Failed to allocate %db\n", fileSize+1);
		orbisClose(pFile);
		return 0;
	}

	if(orbisRead(pFile,pText,fileSize)!=fileSize)
	{
		debugNetPrintf(DEBUG,"[ORBISFILE] Failed to read content of file %s\n", filename);
		orbisClose(pFile);
		free(pText);
		return 0;
	}
	orbisClose(pFile);

	// add null terminator to string
	pText[fileSize] = 0;

	_orbisFile_lastopenFile_size = fileSize;  // now we update exported variable

	return pText;
}
void orbisFileFinish()
{
	int i;
	for(i=0;i<ORBISFILE_MAX_OPEN_DESCRIPTORS;i++)
	{
		if(orbisFileRemoteOpenDescriptors[i]>0)
		{
			debugNetPrintf(DEBUG,"orbisFileRemoteOpenDescriptors[%d] = %d, closing\n", i, orbisFileRemoteOpenDescriptors[i]);
			ps4LinkClose(orbisFileRemoteOpenDescriptors[i]);
		}
		orbisFileRemoteOpenDescriptors[i]=-1;
	}
	orbisfile_initialized=0;
}
int orbisFileInit()
{
	int i;
	if(orbisfile_initialized)
	{
		debugNetPrintf(DEBUG,"[ORBISFILE] liborbisFile already initialized\n");
		return orbisfile_initialized;
	}
	for(i=0;i<ORBISFILE_MAX_OPEN_DESCRIPTORS;i++)
	{
		orbisFileRemoteOpenDescriptors[i]=-1;
	}
	debugNetPrintf(DEBUG,"[ORBISFILE] liborbisFile initialized\n");
	
	orbisfile_initialized=1;
	return orbisfile_initialized;
}
