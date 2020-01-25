/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <kernel.h>
#include <debugnet.h>
#include <orbisNfs.h>
#include <ps4link.h>
#include <nfsc/libnfs.h>
#include <nfsc/libnfs-raw-mount.h>
#include <nfsc/libnfs-raw-nfs.h>
#include <sys/fcntl.h>
#include <string.h>
#include <stdlib.h>


OrbisNfsConfig *orbisNfsConf=NULL;

int orbisnfs_external_conf=-1;

size_t _orbisNfs_lastopenFile_size;

ExtensionTypes myextensionTypes[] = {
	{ ".ROM",  FILE_TYPE_ROM },
	{ ".DSK",  FILE_TYPE_DSK },
	{ ".CAS",  FILE_TYPE_CAS },
	
};
char *mydevices[] = {
	"nfs:",
};
int orbisNfsCheckSlotByFd(int fd)
{
	if(fd<0 || !orbisNfsConf)
	{
		return -1;
	}
	int i = 0;
	int slot = -1;

	// search slot
	for(i=0;i<ORBISNFS_MAX_OPEN_DESCRIPTORS;i++) 
	{ 
		if(orbisNfsConf->openDescriptors[i]==fd) 
		{ 
			slot=i;
			break;
		}
	}
	return slot;
}

int orbisNfsCheckFreeDescriptor()
{
	int i=0;
	int slot=-1;
	// search slot
	for (i=0;i<ORBISNFS_MAX_OPEN_DESCRIPTORS;i++) 
	{ 
		if(orbisNfsConf->openDescriptors[i]==-1) 
		{ 
				slot = i; 
				break; 	
		}
	}
	return slot;
}

int orbisNfsOpen(const char *file, int flags, int mode)
{
	int ret;
	int slot;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s opening file %s\n",__FUNCTION__,file);

	if(!file || !orbisNfsConf)
	{
		return 0x80020016;
	}
	
	slot=orbisNfsCheckFreeDescriptor();
	if(slot>=0)
	{
		if(orbisNfsConf && orbisNfsConf->openEntries[slot])
		{
			ret=nfs_open2(orbisNfsConf->nfsHandle,file,flags,mode,&orbisNfsConf->openEntries[slot]->fileHandle);
			if(ret==0)
			{
				orbisNfsConf->openDescriptors[slot]=orbisNfsConf->openEntries[slot]->fd;
				strncpy(orbisNfsConf->openEntries[slot]->path,file,strlen(file));
				debugNetPrintf(DEBUG,"[ORBISNFS] %s %s opened fd=%d\n",__FUNCTION__,orbisNfsConf->openEntries[slot]->path,orbisNfsConf->openEntries[slot]->fd);
				return orbisNfsConf->openEntries[slot]->fd;
			}
			else
			{
				if(orbisNfsConf->openEntries[slot]->fileHandle)
				{
					free(orbisNfsConf->openEntries[slot]->fileHandle);
					orbisNfsConf->openEntries[slot]->fileHandle=NULL;
				}
				debugNetPrintf(ERROR,"[ORBISNFS] %s unable to open file %s ret=%d %s\n",__FUNCTION__,file,ret,nfs_get_error(orbisNfsConf->nfsHandle));
				return 0x80020016;	
			}
		}
	}
	debugNetPrintf(ERROR,"[ORBISNFS] %s no free slot available\n",__FUNCTION__);
	return 0x80020016;
	
}
int orbisNfsClose(int fd)
{
	int ret=0;
	int slot=-1;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s closing fd %d\n",__FUNCTION__,fd);

	if(fd<0)
	{
		return 0x80020009;
	}
	slot=orbisNfsCheckSlotByFd(fd);
	if(slot>=0)
	{
		//debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d\n",__FUNCTION__,slot,fd);
		if(orbisNfsConf->nfsHandle && orbisNfsConf->openEntries[slot]->fileHandle)
		{
			ret=nfs_close(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->fileHandle);
			if(ret==0)
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d %s closed\n",__FUNCTION__,slot,fd,orbisNfsConf->openEntries[slot]->path);
			}
			else
			{
				debugNetPrintf(ERROR,"[ORBISNFS] %s slot=%d fd=%d closing file %s ret=%d %s\n",__FUNCTION__,slot,fd,orbisNfsConf->openEntries[slot]->path,ret,nfs_get_error(orbisNfsConf->nfsHandle));
			}
			
			orbisNfsConf->openEntries[slot]->fileHandle=NULL;
			
			orbisNfsConf->openDescriptors[slot]=-1;
			memset(orbisNfsConf->openEntries[slot]->path,0,255);

			return 0;
		}
    }
	return 0x80020009;
}

int orbisNfsRead(int fd,void *data,size_t size)
{
	int ret;
	int slot=-1;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s fd=%d size=%d\n",__FUNCTION__,fd,size);

	if(fd<0 || !orbisNfsConf)
	{
		return 0x80020009;
	}
	if(!data)
	{
		return 0x8002000e;
	}
	slot=orbisNfsCheckSlotByFd(fd);
	if(slot>=0)
	{
		if(orbisNfsConf->nfsHandle && orbisNfsConf->openEntries[slot]->fileHandle)
		{
			ret=nfs_read(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->fileHandle,size,(unsigned char *)data);
			if(ret>=0)
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d bytereads=%d\n",__FUNCTION__,slot,fd,ret);
				return ret;
			}
			else
			{
				debugNetPrintf(ERROR,"[ORBISNFS] %s slot=%d fd=%d error=%d %s\n",__FUNCTION__,slot,fd,ret,nfs_get_error(orbisNfsConf->nfsHandle));		
			}
			return 0x80020005;
		}
	}
	return 0x80020009;
}

int orbisNfsWrite(int fd, const void *data, size_t size)
{
	int slot=-1;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s fd=%d size=%d\n",__FUNCTION__,fd,size);
	ssize_t numberOfBytesWritten = 0;
	ssize_t writtenBytes = 0;
	size_t leftBytes = size;
	if(fd<0 || !orbisNfsConf)
	{
		return 0x80020009;
	}
	if(!data)
	{
		return 0x8002000e;
	}
	slot=orbisNfsCheckSlotByFd(fd);
	if(slot>=0)
	{

		if(orbisNfsConf->nfsHandle && orbisNfsConf->openEntries[slot]->fileHandle)
		{
			size_t chunkSize = orbisNfsConf->writeChunkSize > 32768 ? 32768 : orbisNfsConf->writeChunkSize;
			while(leftBytes)
			{
				if(leftBytes < chunkSize)
				{
					chunkSize=leftBytes;//write last chunk with correct size
				}
				//write chunk
				writtenBytes=nfs_write(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->fileHandle,chunkSize,(char *)data+numberOfBytesWritten);
				if(writtenBytes<0)
				{
					debugNetPrintf(ERROR,"[ORBISNFS] %s slot=%d fd=%d error=%d %s\n",__FUNCTION__,slot,fd,writtenBytes,nfs_get_error(orbisNfsConf->nfsHandle));		
					break;
				}
				//decrease left bytes
				leftBytes-= writtenBytes;
				//increase overall written bytes
				numberOfBytesWritten += writtenBytes;

			}
			if(writtenBytes>=0)
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d bytewrite=%d\n",__FUNCTION__,slot,fd,numberOfBytesWritten);
				return numberOfBytesWritten;
			}
			else
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d error=%d %s\n",__FUNCTION__,slot,fd,writtenBytes,nfs_get_error(orbisNfsConf->nfsHandle));				
			}
		}
	}
	return 0x80020009;
}
int orbisNfsLseek(int fd, int offset, int whence)
{
	int ret;
	int slot=-1;
	uint64_t loffset = 0;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s fd=%d offset=%d whence=%d\n",__FUNCTION__,fd,offset,whence);

	if(fd<0 || !orbisNfsConf)
	{
		return 0x80020009;
	}
	slot=orbisNfsCheckSlotByFd(fd);
	if(slot>=0)
	{
		if(orbisNfsConf->nfsHandle && orbisNfsConf->openEntries[slot]->fileHandle)
		{
			ret=(int)nfs_lseek(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->fileHandle,offset,whence,&loffset);
			if(ret<0)
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d error=%d\n",__FUNCTION__,slot,fd,ret,nfs_get_error(orbisNfsConf->nfsHandle));
			}
			else
			{
				debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d fd=%d return=%d\n",__FUNCTION__,slot,fd,(int)loffset);
				return (int)loffset;
			}
		}		
	}
	return 0x80020009;
}

int orbisNfsDopen(const char *path)
{
	int ret;
	int slot;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s path=%s\n",__FUNCTION__,path);

	if(!path || !orbisNfsConf)
	{
		return 0x80020016;
	}
	
		
	slot=orbisNfsCheckFreeDescriptor();
	if(slot>=0)
	{
		if(orbisNfsConf->nfsHandle && !(orbisNfsConf->openEntries[slot]->dirHandle))
		{
			ret=nfs_opendir(orbisNfsConf->nfsHandle,path,&orbisNfsConf->openEntries[slot]->dirHandle);

			if(ret==0)
			{
				orbisNfsConf->openDescriptors[slot]=orbisNfsConf->openEntries[slot]->fd;
				debugNetPrintf(DEBUG,"[ORBISNFS] %s directory %s open slot=%d dfd=%d\n",__FUNCTION__,path,slot,orbisNfsConf->openDescriptors[slot]);
				return orbisNfsConf->openDescriptors[slot];
			}
			else
			{
				free(orbisNfsConf->openEntries[slot]->dirHandle);
				orbisNfsConf->openEntries[slot]->dirHandle=NULL;
			}
			debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d error=%d %s\n",__FUNCTION__,slot,ret,nfs_get_error(orbisNfsConf->nfsHandle));
			return 0x80020016;
		}
	}
	debugNetPrintf(ERROR,"[ORBISNFS] %s no free slot available\n",__FUNCTION__);
	return 0x80020016;
}
int orbisNfsDclose(int dfd)
{
	
	int slot=-1;
	if(dfd<0 || !orbisNfsConf)
	{
		return 0x80020009;
	}
	slot=orbisNfsCheckSlotByFd(dfd);
	if(slot>=0)
	{
		if(orbisNfsConf->nfsHandle && orbisNfsConf->openEntries[slot]->dirHandle)
		{
			nfs_closedir(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->dirHandle);
			
			debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d dfd=%d %s closed\n",__FUNCTION__,slot,dfd,orbisNfsConf->openEntries[slot]->path);
			
			orbisNfsConf->openEntries[slot]->dirHandle=NULL;			
			orbisNfsConf->openDescriptors[slot]=-1;
			strcpy(orbisNfsConf->openEntries[slot]->path,"");
			return 0;
		}
	}
	return 0x80020009;
}
int orbisNfsDread(int dfd,OrbisNfsBrowserDirEntry *entry)
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	int slot=-1;
	unsigned int mode;
	struct nfsdirent *dirent=NULL;
	if(dfd<0)
	{
		return 0x80020009;
	}
	if(!entry)
	{
		return 0x8002000e;
	}
	slot=orbisNfsCheckSlotByFd(dfd);
	if(slot>=0)
	{	
		dirent=nfs_readdir(orbisNfsConf->nfsHandle,orbisNfsConf->openEntries[slot]->dirHandle);
		
		if(dirent!=NULL)
		{
			mode = (dirent->mode & 0xFFF);
			switch(dirent->type)
			{
				case NF3REG:
					entry->type=DT_REG;
					mode |= 0x0100000; 
					break;
				case NF3DIR:
					entry->type=DT_DIR;
					mode |= 0x0040000;
					break;
				case NF3LNK:
					entry->type=DT_LNK;
					mode |= 0x0120000;
					break;
				default:
					entry->type=DT_UNKNOWN;
					break;
			}
			
			
			entry->customtype=0;
			entry->mode=mode;
			entry->namelen=strlen(dirent->name);
			memset(entry->name,0,255);
			strncpy(entry->name,dirent->name,entry->namelen);
			struct tm *cloctime,*aloctime,*mloctime;
			cloctime=localtime(&dirent->ctime.tv_sec);
			aloctime=localtime(&dirent->atime.tv_sec);
			mloctime=localtime(&dirent->mtime.tv_sec);

			entry->ctime.year = (unsigned short)(cloctime->tm_year+1900);
			entry->ctime.month = (unsigned short)(cloctime->tm_mon + 1);
			entry->ctime.day = (unsigned short)cloctime->tm_mday;
			entry->ctime.hour = (unsigned short)cloctime->tm_hour;
			entry->ctime.minute = (unsigned short)cloctime->tm_min;
			entry->ctime.second = (unsigned short)cloctime->tm_sec;

			entry->atime.year = (unsigned short)(aloctime->tm_year+1900);
			entry->atime.month = (unsigned short)(aloctime->tm_mon + 1);
			entry->atime.day = (unsigned short)aloctime->tm_mday;
			entry->atime.hour = (unsigned short)aloctime->tm_hour;
			entry->atime.minute = (unsigned short)aloctime->tm_min;
			entry->atime.second = (unsigned short)aloctime->tm_sec;

			entry->mtime.year = (unsigned short)(mloctime->tm_year+1900);
			entry->mtime.month = (unsigned short)(mloctime->tm_mon + 1);
			entry->mtime.day = (unsigned short)mloctime->tm_mday;
			entry->mtime.hour = (unsigned short)mloctime->tm_hour;
			entry->mtime.minute = (unsigned short)mloctime->tm_min;
			entry->mtime.second = (unsigned short)mloctime->tm_sec;
			

			entry->size=(unsigned int)dirent->size;
			debugNetPrintf(DEBUG,"[ORBISNFS] %s slot=%d dfd=%d entry %s size=%d\n",__FUNCTION__,slot,dfd,entry->name,entry->size);

			return 1;
		}
		else
		{
			return 0;
		}
		
	}
	return 0x80020009;
	
}
/*
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
*/




unsigned char * orbisNfsGetFileContent(const char *filename)
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	_orbisNfs_lastopenFile_size = -1;  // reset exported variable

	int pFile=orbisNfsOpen(filename,O_RDONLY,0);

	if(pFile<0)
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s Failed to read file %s\n",__FUNCTION__,filename);
		return 0;
	}

	// obtain file size:
	int32_t fileSize=orbisNfsLseek(pFile,0,SEEK_END);
	orbisNfsLseek(pFile,0,SEEK_SET);  // Seek back to start
	if(fileSize<0)
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s Failed to read size of file %s\n",__FUNCTION__,filename);
		orbisNfsClose(pFile);
		return NULL;
	}

	unsigned char* pText=malloc(sizeof(unsigned char)*fileSize+1);
	if(!pText)
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s Failed to allocate %d\n",__FUNCTION__,fileSize+1);
		orbisNfsClose(pFile);
		return 0;
	}

	if(orbisNfsRead(pFile,pText,fileSize)!=fileSize)
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s Failed to read content of file %s\n",__FUNCTION__,filename);
		orbisNfsClose(pFile);
		free(pText);
		return 0;
	}
	orbisNfsClose(pFile);

	// add null terminator to string
	pText[fileSize] = 0;

	_orbisNfs_lastopenFile_size = fileSize;  // now we update exported variable

	return pText;
}

int orbisNfsBrowserGetDirLevel()
{
	return orbisNfsConf->dirLevel;
}
int orbisNfsBrowserGetBasePos()
{
	return orbisNfsConf->basePos;
}
int orbisNfsBrowserGetRelPos()
{
	return orbisNfsConf->relPos;
}
int orbisNfsBrowserGetListLength()
{
	return orbisNfsConf->browserList->length;
}
char* orbisNfsBrowserGetListPath()
{
	return orbisNfsConf->browserList->path;
}
void orbisNfsBrowserSetListPath1(char *path)
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	int len = strlen(orbisNfsConf->browserList->path);
	debugNetPrintf(DEBUG,"[ORBISNFS] %s %s len=%d\n",__FUNCTION__,orbisNfsConf->browserList->path,len);
	
	if (orbisNfsConf->dirLevel==0) 
	{
		strcpy(orbisNfsConf->browserList->path, path);
	} 
	else 
	{
		//if(orbisNfsConf->dirLevel > 1)
		//{
			if(len+strlen(path)<MAX_PATH_LENGTH-2) 
			{
				if(orbisNfsConf->browserList->path[len-1]!='/') 
				{
					strcat(orbisNfsConf->browserList->path,"/");
				}
			}
		//}
		strcat(orbisNfsConf->browserList->path, path);
		debugNetPrintf(DEBUG,"[ORBISNFS] %s concat done %s len=%d\n",__FUNCTION__,orbisNfsConf->browserList->path,strlen(orbisNfsConf->browserList->path));
		
	}
}

void orbisNfsBrowserSetListPath(char *path)
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	int len = strlen(orbisNfsConf->browserList->path);
	debugNetPrintf(DEBUG,"[ORBISNFS] %s %s len=%d dirlevel=%d\n",__FUNCTION__,orbisNfsConf->browserList->path,len,orbisNfsConf->dirLevel);
	
	if(orbisNfsConf->dirLevel==0) 
	{
		strcpy(orbisNfsConf->browserList->path, path);
	} 
	else 
	{
		if(orbisNfsConf->dirLevel > 1)
		{
			if(len+strlen(path)<MAX_PATH_LENGTH-2) 
			{
				if(orbisNfsConf->browserList->path[len-1]!='/') 
				{
					strcat(orbisNfsConf->browserList->path,"/");
				}
			}
		}
		strcat(orbisNfsConf->browserList->path, path);
		debugNetPrintf(DEBUG,"[ORBISNFS] %s concat done %s len=%d\n",__FUNCTION__,orbisNfsConf->browserList->path,strlen(orbisNfsConf->browserList->path));
	}
}
void orbisNfsBrowserEntryDown()
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if((orbisNfsConf->relPos+1)<orbisNfsConf->browserList->length) 
	{
		if((orbisNfsConf->relPos+1)<MAX_POSITION) 
		{
			orbisNfsConf->relPos++;
		} 
		else 
		{
			if((orbisNfsConf->basePos+orbisNfsConf->relPos+1)<orbisNfsConf->browserList->length) 
			{
				orbisNfsConf->basePos++;
			}
		}
	}
}
void orbisNfsBrowserEntryUp() 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(orbisNfsConf->relPos>0) 
	{
		orbisNfsConf->relPos--;
	} 
	else 
	{
		if(orbisNfsConf->basePos>0) 
		{
			orbisNfsConf->basePos--;
		}
	}
}
int orbisNfsBrowserGetFileType(char *file) 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	char *p=strrchr(file,'.');
	if(p) 
	{
		int i;
		for(i=0;i<(sizeof(myextensionTypes)/sizeof(ExtensionTypes));i++) 
		{
			if(strcasecmp(p,myextensionTypes[i].extension)==0) 
			{
				return myextensionTypes[i].type;
			}
		}
	}

	return FILE_TYPE_UNKNOWN;
}
void orbisNfsBrowserDirLevelUp(char *path) 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);
	orbisNfsConf->basePosList[orbisNfsConf->dirLevel]=orbisNfsConf->basePos;
	orbisNfsConf->relPosList[orbisNfsConf->dirLevel]=orbisNfsConf->relPos;
	orbisNfsConf->dirLevel++;
	orbisNfsConf->basePosList[orbisNfsConf->dirLevel]=0;
	orbisNfsConf->relPosList[orbisNfsConf->dirLevel]=0;
	orbisNfsConf->basePos=0;
	orbisNfsConf->relPos=0;
	orbisNfsBrowserSetListPath(path);
	orbisNfsBrowserListRefresh();
}
void orbisNfsBrowserDirLevelDown() 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	char *p;

	p=strrchr(orbisNfsConf->browserList->path,'/');
	if(p) 
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s before %s %d\n",__FUNCTION__,orbisNfsConf->browserList->path,strlen(orbisNfsConf->browserList->path));
		p[0]='\0';
		if(orbisNfsConf->dirLevel>0)
		orbisNfsConf->dirLevel--;
		debugNetPrintf(DEBUG,"[ORBISNFS] %s after %s %d\n",__FUNCTION__,orbisNfsConf->browserList->path,strlen(orbisNfsConf->browserList->path));
		
		goto DIR_UP_RETURN;
	}

	p=strrchr(orbisNfsConf->browserList->path,':');
	if(p) 
	{
		if (strlen(orbisNfsConf->browserList->path)-((p+1)-orbisNfsConf->browserList->path)>0) 
		{
			p[1]='\0';
			if(orbisNfsConf->dirLevel>0)
			orbisNfsConf->dirLevel--;
			goto DIR_UP_RETURN;
		}
	}

	strcpy(orbisNfsConf->browserList->path, ROOT_PATH);
	orbisNfsConf->dirLevel=0;
	

DIR_UP_RETURN:
	orbisNfsConf->basePos=orbisNfsConf->basePosList[orbisNfsConf->dirLevel];
	orbisNfsConf->relPos=orbisNfsConf->relPosList[orbisNfsConf->dirLevel];
	orbisNfsBrowserListRefresh();
}

void orbisNfsBrowserListClean()
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(orbisNfsConf)
	{

		if(orbisNfsConf->browserList)
		{
		
			OrbisNfsBrowserListEntry *entry = orbisNfsConf->browserList->head;

			while (entry)
			{
				OrbisNfsBrowserListEntry *next = entry->next;
				debugNetPrintf(DEBUG,"[ORBISNFS] %s before free %s\n",__FUNCTION__,entry->dir->name);
			
				free(entry->dir);
				debugNetPrintf(DEBUG,"[ORBISNFS] %s after free dir\n",__FUNCTION__);
			
				free(entry);
				debugNetPrintf(DEBUG,"[ORBISNFS] %s after free entry\n",__FUNCTION__);
			
				entry = next;
			}
			debugNetPrintf(DEBUG,"[ORBISNFS] %s cleaning head tail and length\n",__FUNCTION__);

			orbisNfsConf->browserList->head=NULL;
			orbisNfsConf->browserList->tail=NULL;
			orbisNfsConf->browserList->length=0;
		}
	}
	
}

int orbisNfsBrowserListRefresh() 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	int ret=0,res=0;

	//do 
	//{
		orbisNfsBrowserListClean();
		//debugNetPrintf(DEBUG,"after orbisFilebrowserListClean\n");

		res=orbisNfsBrowserListGetEntries(orbisNfsConf->browserList->path);
		debugNetPrintf(DEBUG,"[ORBISNFS] %s (%s) ret:%d\n",__FUNCTION__ ,orbisNfsConf->browserList->path, res);

		if(res<0) 
		{
			debugNetPrintf(ERROR,"[ORBISNFS] %s error=%d\n",__FUNCTION__,res);
			
			ret=res;
			orbisNfsBrowserDirLevelDown();
		}
		//}while(res<0);

	// Correct position after deleting the latest entry of the file list
	while((orbisNfsConf->basePos+orbisNfsConf->relPos)>=orbisNfsConf->browserList->length) 
	{
		if(orbisNfsConf->basePos>0) 
		{
			orbisNfsConf->basePos--;
		} 
		else 
		{
			if(orbisNfsConf->relPos>0) 
			{
				orbisNfsConf->relPos--;
			}
		}
	}

	// Correct position after deleting an entry while the scrollbar is on the bottom
	if(orbisNfsConf->browserList->length>=MAX_POSITION) {
		while((orbisNfsConf->basePos+MAX_POSITION-1)>=orbisNfsConf->browserList->length)
		{
			if(orbisNfsConf->basePos>0)
			{
				orbisNfsConf->basePos--;
				orbisNfsConf->relPos++;
			}
		}
	}

	return ret;
}

OrbisNfsBrowserListEntry *orbisNfsBrowserListGetNthEntry(int n) 
{
	debugNetPrintf(0,"[ORBISNFS] %s\n",__FUNCTION__);

	OrbisNfsBrowserListEntry *entry; 
	if(!orbisNfsConf || !orbisNfsConf->browserList)
	{
		return NULL;
	}
	entry=orbisNfsConf->browserList->head;
	while(n>0 && entry->next)
	{
		n--;
		entry=entry->next;
	}

	if (n != 0)
		return NULL;

	return entry;
}

void orbisNfsBrowserListAddEntry(OrbisNfsBrowserListEntry *entry,int sort) 
{
	if(!orbisNfsConf || !entry)
	{
		return;
	}
	debugNetPrintf(DEBUG,"[ORBISNFS] %s %s type=%d customType=%d\n",__FUNCTION__,entry->dir->name,entry->dir->type,entry->dir->customtype);

	if(orbisNfsConf->browserList)
	{
		entry->next=NULL;
		entry->previous=NULL;
		if(orbisNfsConf->browserList->head==NULL) 
		{
			orbisNfsConf->browserList->head=entry;
			orbisNfsConf->browserList->tail=entry;
		} 
		else 
		{
			if(strcmp(entry->dir->name,"..")==0)
			{
				entry->next=orbisNfsConf->browserList->head;
				orbisNfsConf->browserList->head=entry;
				
			}
			else
			{
				if(strcmp(entry->dir->name,".")==0)
				{
					entry->next=orbisNfsConf->browserList->head;
					orbisNfsConf->browserList->head=entry;
				}
				else
				{
					OrbisNfsBrowserListEntry *tail=orbisNfsConf->browserList->tail;
					tail->next=entry;
					entry->previous=tail;
					orbisNfsConf->browserList->tail=entry;
				}
			}
			
		}

		orbisNfsConf->browserList->length++;
	}
}

int orbisNfsBrowserGetDirectoryEntries(char *path) 
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	int type=0;
	int res=0;
	int dfd=orbisNfsDopen(path);
	if (dfd<0)
		return dfd;	
	do
	{
		OrbisNfsBrowserDirEntry *dir;
		dir=malloc(sizeof(OrbisNfsBrowserDirEntry));
		memset(dir->name,0,255);
		res=orbisNfsDread(dfd,dir);
		if(res>0)
		{
			/* skip useless backup (~) files! */
			if(strstr(dir->name, "~")) continue;

			OrbisNfsBrowserListEntry *entry=malloc(sizeof(OrbisNfsBrowserListEntry));
			entry->dir=dir;
			if(dir->type!=DT_DIR)
			{
				type=orbisNfsBrowserGetFileType(entry->dir->name);
				switch(type)
				{
					case FILE_TYPE_ROM:
						if(strncmp(path,"./system",6)==0)
						{
							entry->dir->customtype=FILE_TYPE_SYSTEM_ROM;
						}
						else
						{
							entry->dir->customtype=FILE_TYPE_GAME_ROM;	
						}
						break;
					case FILE_TYPE_DSK:
						entry->dir->customtype=FILE_TYPE_GAME_DSK;
						break;
					case FILE_TYPE_CAS:
						entry->dir->customtype=FILE_TYPE_CAS;
						break;
					case FILE_TYPE_UNKNOWN:
						entry->dir->customtype=FILE_TYPE_UNKNOWN;
						break;
				}
			}
			else
			{
				entry->dir->customtype=FILE_TYPE_FOLDER;
			}
			orbisNfsBrowserListAddEntry(entry,SORT_BY_NAME_AND_FOLDER);
				
		}
				
	}
	while(res>0);
		//res=0;
		
	
	orbisNfsDclose(dfd);
	return res;
}

void orbisNfsFinish()
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(orbisnfs_external_conf!=1)
	{
		if(orbisNfsConf->orbisnfs_initialized==1)
		{		
			nfs_destroy_url(orbisNfsConf->urls);
			if(orbisNfsConf->nfsHandle!=NULL)
			{
				nfs_destroy_context(orbisNfsConf->nfsHandle);
				orbisNfsBrowserListClean();
				if(orbisNfsConf->browserList)
				free(orbisNfsConf->browserList);
			}
		}
		orbisNfsConf->orbisnfs_initialized=-1;
	
		debugNetPrintf(DEBUG,"[ORBISNFS] %s liborbisNfs finished\n",__FUNCTION__);
	}
	orbisNfsConf->orbisnfs_initialized=0;
}
int orbisNfsBrowserGetDevices()
{
	int i;
	for(i=0;i<NUM_DEVICES;i++) 
	{
		if(mydevices[i]) 
		{
			
				OrbisNfsBrowserListEntry *entry=malloc(sizeof(OrbisNfsBrowserListEntry));
				strcpy(entry->dir->name,mydevices[i]);
				entry->dir->customtype=FILE_TYPE_FOLDER;
				orbisNfsBrowserListAddEntry(entry,SORT_BY_NAME_AND_FOLDER);
			
		}
	}

	return 0;
}
int orbisNfsBrowserListGetEntries(char *path)
{
	
	if(strcmp(path,ROOT_PATH)==0) 
	{
		return orbisNfsBrowserGetDevices();
	}

	return orbisNfsBrowserGetDirectoryEntries(path);
	
}
OrbisNfsConfig *orbisNfsGetConf()
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(orbisNfsConf)
	{
		return orbisNfsConf;
	}
	
	return NULL; 
}

int orbisNfsSetConf(OrbisNfsConfig *conf)
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(conf)
	{
		orbisNfsConf=conf;
		orbisnfs_external_conf=1;
		return orbisNfsConf->orbisnfs_initialized;
	}
	else
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s invalid conf\n",__FUNCTION__);
	}	
	return 0; 
}
int orbisNfsInitWithConf(OrbisNfsConfig *conf)
{
	int ret;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	ret=orbisNfsSetConf(conf);
	if(ret)
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s liborbisNfs already initialized using external configuration\n",__FUNCTION__);
		debugNetPrintf(DEBUG,"[ORBISNFS] %s orbisnfs_initialized=%d\n",__FUNCTION__,orbisNfsConf->orbisnfs_initialized);
		debugNetPrintf(DEBUG,"[ORBISNFS] %s ready to have a lot of fun...\n",__FUNCTION__);
		return orbisNfsConf->orbisnfs_initialized;
	}
	else
	{
		return 0;
	}
}

int orbisNfsCreateConf()
{
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);
	int i;
	if(!orbisNfsConf)
	{
		orbisNfsConf=(OrbisNfsConfig *)malloc(sizeof(OrbisNfsConfig));
		if(orbisNfsConf)
		{
			orbisNfsConf->nfsHandle=NULL;
			orbisNfsConf->urls=NULL;
			for(i=0;i<ORBISNFS_MAX_OPEN_DESCRIPTORS;i++)
			{
				orbisNfsConf->openDescriptors[i]=-1;
				orbisNfsConf->openEntries[i]=(OrbisNfsEntry *)malloc(sizeof(OrbisNfsEntry));
				if(orbisNfsConf->openEntries[i])
				{
					orbisNfsConf->openEntries[i]->fd=i;
					orbisNfsConf->openEntries[i]->size=0;
					orbisNfsConf->openEntries[i]->fileHandle=NULL;
					orbisNfsConf->openEntries[i]->dirHandle=NULL;
					orbisNfsConf->browserList=NULL;
					orbisNfsConf->basePos=0;
					orbisNfsConf->relPos=0;
					orbisNfsConf->dirLevel=0;

				}
				else
				{
					debugNetPrintf(ERROR,"[ORBISNFS] %s malloc error 2 in %d\n",__FUNCTION__,i);
					return 1;
				}
			}
			orbisNfsConf->orbisnfs_initialized=-1;
			return 0;
		}
		else
		{
			debugNetPrintf(ERROR,"[ORBISNFS] %s malloc error 1\n",__FUNCTION__);
			return 1;
		}
	}
	if(orbisNfsConf->orbisnfs_initialized==1)
	{
		return orbisNfsConf->orbisnfs_initialized;
	}
	//something weird happened
	return -1;	
}
char * orbisNfsGetExportPath()
{

	return orbisNfsConf->urls->path;
}
int orbisNfsInit(const char *nfsurl)
{
	int ret;
	debugNetPrintf(DEBUG,"[ORBISNFS] %s\n",__FUNCTION__);

	if(nfsurl==NULL)
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s invalid url\n",__FUNCTION__);
		return -1;
	}
	
	if(orbisNfsCreateConf()==1)
	{
			return orbisNfsConf->orbisnfs_initialized;
	}
	
	if(orbisNfsConf->orbisnfs_initialized==1) 
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s liborbisNfs is already initialized!\n",__FUNCTION__);
		return orbisNfsConf->orbisnfs_initialized;
	}
	
	debugNetPrintf(DEBUG,"[ORBISNFS] %s calling nfs_init_context\n",__FUNCTION__);

	orbisNfsConf->nfsHandle=nfs_init_context();

	if(orbisNfsConf->nfsHandle==NULL)
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s Failed to init nfs context %s\n",__FUNCTION__,nfs_get_error(orbisNfsConf->nfsHandle));
		return orbisNfsConf->orbisnfs_initialized;
	}
	
	debugNetPrintf(DEBUG,"[ORBISNFS] %s nfs_init_context return valid nfs object\n",__FUNCTION__);
	
	debugNetPrintf(DEBUG,"[ORBISNFS] %s calling nfs_parse_url_dir\n",__FUNCTION__);

	orbisNfsConf->urls=nfs_parse_url_dir(orbisNfsConf->nfsHandle,nfsurl);
	
	if(orbisNfsConf->urls==NULL) 
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s Failed to parse url : %s\n",__FUNCTION__,nfs_get_error(orbisNfsConf->nfsHandle));
		return orbisNfsConf->orbisnfs_initialized;
	}
	
	debugNetPrintf(DEBUG,"[ORBISNFS] %s nfs_parse_url_dir return valid urls object\n",__FUNCTION__);

	debugNetPrintf(DEBUG,"[ORBISNFS] %s calling nfs_mount\n",__FUNCTION__);
	
	ret=nfs_mount(orbisNfsConf->nfsHandle,orbisNfsConf->urls->server,orbisNfsConf->urls->path);
	
	if(ret!=0) 
	{
		debugNetPrintf(DEBUG,"[ORBISNFS] %s Failed to mount nfs share : %s\n",__FUNCTION__,nfs_get_error(orbisNfsConf->nfsHandle));
		return orbisNfsConf->orbisnfs_initialized;
	}
	debugNetPrintf(DEBUG,"[ORBISNFS] %s nfs_mount return fine\n",__FUNCTION__);

	orbisNfsConf->readChunkSize=nfs_get_readmax(orbisNfsConf->nfsHandle);
    orbisNfsConf->writeChunkSize=nfs_get_writemax(orbisNfsConf->nfsHandle);
	
	orbisNfsConf->orbisnfs_initialized=1;
	
	debugNetPrintf(DEBUG,"[ORBISNFS] %s liborbisNfs initialized readChunkSize=%d writeChunkSize=%d\n",__FUNCTION__,orbisNfsConf->readChunkSize,orbisNfsConf->writeChunkSize);

	orbisNfsConf->browserList=malloc(sizeof(OrbisNfsBrowserList));
	if(orbisNfsConf->browserList)
	{
		orbisNfsBrowserListClean();
		strcpy(orbisNfsConf->browserList->path, ".");
		orbisNfsConf->dirLevel=1;
		//orbisNfsBrowserDirLevelUp();				
		orbisNfsBrowserListGetEntries(".");	
	}
	else
	{
		debugNetPrintf(ERROR,"[ORBISNFS] %s problem in malloc browserList\n",__FUNCTION__);

	}
	return orbisNfsConf->orbisnfs_initialized;
}