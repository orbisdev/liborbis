/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/liborbis
 */
#ifndef _ORBISFILEBROWSER_H_
#define _ORBISFILEBROWSER_H_

#define ROOT_PATH "root"
#define DIR_UP ".."
#define DIR_ACT "."

#define MAX_PATH_LENGTH 1024
#define MAX_NAME_LENGTH 256
#define MAX_SHORT_NAME_LENGTH 64
#define MAX_MOUNT_POINT_LENGTH 2

// Max entries
#define MAX_POSITION 16
#define MAX_ENTRIES 17
#define MAX_DIR_LEVELS 1024

enum FileTypes {
	FILE_TYPE_UNKNOWN,
	FILE_TYPE_FOLDER,
	FILE_TYPE_ROM,
	FILE_TYPE_SYSTEM_ROM,
	FILE_TYPE_GAME_ROM,
	FILE_TYPE_DSK,
	FILE_TYPE_GAME_DSK,
	FILE_TYPE_CAS,
	FILE_TYPE_CONFIG,
};


enum SortFlags {
	SORT_NONE,
	SORT_BY_NAME_AND_FOLDER,
};


#define NUM_DEVICES (sizeof(devices) / sizeof(char **))


typedef struct OrbisFileBrowserListEntry {
	struct OrbisFileBrowserListEntry *next;
	struct OrbisFileBrowserListEntry *previous;
	char name[MAX_NAME_LENGTH];
	int type;
} OrbisFileBrowserListEntry;

typedef struct OrbisFileBrowserList {
	OrbisFileBrowserListEntry *head;
	OrbisFileBrowserListEntry *tail;
	int length;
	char path[MAX_PATH_LENGTH];
} OrbisFileBrowserList;

typedef struct ExtensionType {
	char *extension;
	int type;
} ExtensionType;

int orbisFileBrowserGetDirLevel();
int orbisFileBrowserGetBasePos();
int orbisFileBrowserGetRelPos();
int orbisFileBrowserGetListLength();
void orbisFileBrowserSetListPath(char *path);
void orbisFileBrowserEntryDown();
void orbisFileBrowserEntryUp();
int orbisFileBrowserGetFileType(char *file);
void orbisFileBrowserDirLevelUp();
void orbisFileBrowserDirUp();
void orbisFileBrowserListClean();
int orbisFileBrowserListRefresh();
OrbisFileBrowserListEntry *orbisFileBrowserListGetNthEntry(int n);
void orbisFileBrowserListAddEntry(OrbisFileBrowserListEntry *entry,int sort);
int orbisFileBrowserGetDirectoryEntries(char *path);
int orbisFileBrowserGetDevices();
int orbisFileBrowserListGetEntries(char *path);
int orbisFileBrowserInit(char *path);
void orbisFileBrowserFinish();





#endif



