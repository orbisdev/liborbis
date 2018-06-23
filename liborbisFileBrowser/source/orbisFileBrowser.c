/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>
#include <string.h>
#include <ps4link.h>
#include <debugnet.h>
#include "orbisFileBrowser.h"

int orbisFileBrowserInitialized=0;
OrbisFileBrowserList *browserList;

// Position
int basePos = 0, relPos = 0;
int basePosList[MAX_DIR_LEVELS];
int relPosList[MAX_DIR_LEVELS];
int dirLevel=0;

char *devices[] = {
	"host0:",
};

ExtensionType extensionTypes[] = {
	{ ".ROM",  FILE_TYPE_ROM },
	{ ".DSK",  FILE_TYPE_DSK },
	{ ".CAS",  FILE_TYPE_CAS },
	
};
int orbisFileBrowserGetDirLevel()
{
	return dirLevel;
}
int orbisFileBrowserGetBasePos()
{
	return basePos;
}
int orbisFileBrowserGetRelPos()
{
	return relPos;
}
int orbisFileBrowserGetListLength()
{
	return browserList->length;
}
void orbisFileBrowserSetListPath(char *path)
{
	int len = strlen(browserList->path);
	if (dirLevel==0) 
	{
		strcpy(browserList->path, path);
	} 
	else 
	{
		if(dirLevel > 1)
		{
			if(len<MAX_PATH_LENGTH-2) 
			{
				if(browserList->path[len-1]!='/') 
				{
					strcat(browserList->path,"/");
				}
			}
		}
		strcat(browserList->path, path);
	}
}
void orbisFileBrowserEntryDown()
{
	if((relPos+1)<browserList->length) 
	{
		if((relPos+1)<MAX_POSITION) 
		{
			relPos++;
		} 
		else 
		{
			if((basePos+relPos+1)<browserList->length) 
			{
				basePos++;
			}
		}
	}
}
void orbisFileBrowserEntryUp() 
{
	if(relPos>0) 
	{
		relPos--;
	} 
	else 
	{
		if(basePos>0) 
		{
			basePos--;
		}
	}
}
int orbisFileBrowserGetFileType(char *file) 
{
	char *p=strrchr(file,'.');
	if(p) 
	{
		int i;
		for(i=0;i<(sizeof(extensionTypes)/sizeof(ExtensionType));i++) 
		{
			if(strcasecmp(p,extensionTypes[i].extension)==0) 
			{
				return extensionTypes[i].type;
			}
		}
	}

	return FILE_TYPE_UNKNOWN;
}
void orbisFileBrowserDirLevelUp() 
{
	basePosList[dirLevel]=basePos;
	relPosList[dirLevel]=relPos;
	dirLevel++;
	basePosList[dirLevel]=0;
	relPosList[dirLevel]=0;
	basePos=0;
	relPos=0;
}
void orbisFileBrowserDirUp() 
{

	char *p;

	p=strrchr(browserList->path,'/');
	if(p) 
	{
		p[1]='\0';
		dirLevel--;
		goto DIR_UP_RETURN;
	}

	p=strrchr(browserList->path,':');
	if(p) 
	{
		if (strlen(browserList->path)-((p+1)-browserList->path)>0) 
		{
			p[1]='\0';
			dirLevel--;
			goto DIR_UP_RETURN;
		}
	}

	strcpy(browserList->path, ROOT_PATH);
	dirLevel=0;

DIR_UP_RETURN:
	basePos=basePosList[dirLevel];
	relPos=relPosList[dirLevel];
}

void orbisFileBrowserListClean()
{
	if(browserList)
	{
		if(browserList->head)
		{	OrbisFileBrowserListEntry *entry=browserList->head;
			while(entry) 
			{
				if(entry->next)
				{
					OrbisFileBrowserListEntry *next=entry->next;
					free(entry);
					entry=next;
				}
			}
		}
		browserList->head=NULL;
		browserList->tail=NULL;
		browserList->length=0;
	}
	
}
int orbisFileBrowserListRefresh() 
{
	int ret=0,res=0;

	do 
	{
		orbisFileBrowserListClean();

		res=orbisFileBrowserListGetEntries(browserList->path);

		if(res<0) 
		{
			ret=res;
			orbisFileBrowserDirUp();
		}
	}while(res<0);

	// Correct position after deleting the latest entry of the file list
	while((basePos+relPos)>=browserList->length) 
	{
		if(basePos>0) 
		{
			basePos--;
		} 
		else 
		{
			if(relPos>0) 
			{
				relPos--;
			}
		}
	}

	// Correct position after deleting an entry while the scrollbar is on the bottom
	if(browserList->length>=MAX_POSITION) {
		while((basePos+MAX_POSITION-1)>=browserList->length)
		{
			if(basePos>0)
			{
				basePos--;
				relPos++;
			}
		}
	}

	return ret;
}
OrbisFileBrowserListEntry *orbisFileBrowserListGetNthEntry(int n) 
{
	OrbisFileBrowserListEntry *entry; 
	int i;
	if(n<browserList->length/2)
	{
		entry=browserList->head;
		for(i=0;i<n;i++) 
		{
			entry=entry->next;
		}
	}
	else
	{
		entry=browserList->tail;
		for(i=browserList->length-1;i>n;i--)
		{
			entry=entry->previous;
		}
		
	}
	return entry;
}

void orbisFileBrowserListAddEntry(OrbisFileBrowserListEntry *entry,int sort) 
{
	entry->next=NULL;
	entry->previous=NULL;

	if(browserList->head==NULL) 
	{
		browserList->head=entry;
		browserList->tail=entry;
	} 
	else 
	{
		if(sort!=SORT_NONE) 
		{
			int entry_length=strlen(entry->name);

			OrbisFileBrowserListEntry *p=browserList->head;
			OrbisFileBrowserListEntry *previous=NULL;

			while(p) 
			{
				// Sort by type
				if(entry->type>p->type)
					break;

				// '..' is always at first
				if(strcmp(entry->name,"..")==0)
					break;

				if(strcmp(p->name,"..")!=0) 
				{
					// Get the minimum length without /
					int name_length=strlen(p->name);
					int len=(((entry_length) < (name_length)) ? (entry_length) : (name_length));
					if(entry->name[len - 1]=='/' || p->name[len - 1]=='/')
						len--;

					// Sort by name
					if(entry->type==p->type) 
					{
						int diff=strncasecmp(entry->name,p->name,len);
						if (diff<0 || (diff==0 && entry_length<name_length)) 
						{
							break;
						}
					}
				}

				previous=p;
				p=p->next;
			}

			if(previous==NULL) 
			{ // Order: entry (new head) -> p (old head)
				entry->next=p;
				p->previous=entry;
				browserList->head=entry;
			} 
			else if(previous->next == NULL) 
			{ // Order: p (old tail) -> entry (new tail)
				OrbisFileBrowserListEntry *tail=browserList->tail;
				tail->next=entry;
				entry->previous=tail;
				browserList->tail=entry;
			} 
			else 
			{ // Order: previous -> entry -> p
				previous->next=entry;
				entry->previous=previous; 
				entry->next=p;
				p->previous=entry;
			}
		} 
		else 
		{
			OrbisFileBrowserListEntry *tail=browserList->tail;
			tail->next=entry;
			browserList->tail=entry;
		}
	}

	browserList->length++;
}
int orbisFileBrowserGetDirectoryEntries(char *path) 
{
	int type=0;
	int dfd=ps4LinkDopen(path);
	if (dfd<0)
		return dfd;

	OrbisFileBrowserListEntry *entry=malloc(sizeof(OrbisFileBrowserListEntry));
	strcpy(entry->name,DIR_UP);
	entry->type=FILE_TYPE_FOLDER;
	orbisFileBrowserListAddEntry(entry,SORT_BY_NAME_AND_FOLDER);

	int res=0;

	do 
	{
		struct dirent dir;
		memset(&dir,0,sizeof(struct dirent));

		res=ps4LinkDread(dfd,&dir);
		if(res>0) 
		{
			if(strcmp(dir.d_name,".")==0 || strcmp(dir.d_name,"..")==0)
				continue;

			OrbisFileBrowserListEntry *entry=malloc(sizeof(OrbisFileBrowserListEntry));

			strcpy(entry->name,dir.d_name);

			if(dir.d_type!=DT_DIR)
			{			
				type=orbisFileBrowserGetFileType(entry->name);
				if(type==FILE_TYPE_ROM)
				{
					if(strcmp(path,"system")==0)
					{
						entry->type=FILE_TYPE_SYSTEM_ROM;
					}
					else
					{
						entry->type=FILE_TYPE_GAME_ROM;
						
					}
				}
				if(type==FILE_TYPE_DSK)
				{
					entry->type=FILE_TYPE_GAME_DSK;
					
				}
				if(type==FILE_TYPE_CAS)
				{
					entry->type=FILE_TYPE_CAS;
					
				}
			}
			else
			{
				entry->type=dir.d_type;
			}
			orbisFileBrowserListAddEntry(entry,SORT_BY_NAME_AND_FOLDER);
		}
	}while(res>0);

	ps4LinkDclose(dfd);

	return 0;
}

int orbisFileBrowserGetDevices()
{
	int i;
	for(i=0;i<NUM_DEVICES;i++) 
	{
		if(devices[i]) 
		{
			
				OrbisFileBrowserListEntry *entry=malloc(sizeof(OrbisFileBrowserListEntry));
				strcpy(entry->name,devices[i]);
				entry->type=FILE_TYPE_FOLDER;
				orbisFileBrowserListAddEntry(entry,SORT_BY_NAME_AND_FOLDER);
			
		}
	}

	return 0;
}
int orbisFileBrowserListGetEntries(char *path)
{
	
	if(strcmp(path,ROOT_PATH)==0) 
	{
		return orbisFileBrowserGetDevices();
	}

	return orbisFileBrowserGetDirectoryEntries(path);
	
}
int orbisFileBrowserInit(char *path) 
{
	if(!orbisFileBrowserInitialized)
	{
		
			browserList=malloc(sizeof(OrbisFileBrowserList));
			if(browserList)
			{
				orbisFileBrowserListClean();
				if(path==NULL)
				{
					strcpy(browserList->path, ROOT_PATH);	
				}
				else
				{
					strcpy(browserList->path, path);	
					
				}
				orbisFileBrowserListGetEntries(browserList->path);
				
			}
			else
			{
				return -1;
			}
			orbisFileBrowserInitialized=1;
	}
	else
	{
		orbisFileBrowserListClean();
	}
	strcpy(browserList->path, ROOT_PATH);	
	return orbisFileBrowserInitialized;
}
void orbisFileBrowserFinish() 
{
	orbisFileBrowserListClean();
	if(browserList)
	free(browserList);
	orbisFileBrowserInitialized=0;
}
