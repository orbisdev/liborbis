/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#ifndef _ORBISNFS_H_
#define _ORBISNFS_H_

#include <stdio.h> // SEEK_*
#include <dirent.h>
#include <nfsc/libnfs.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ORBISNFS_MAX_OPEN_DESCRIPTORS 10

#define ROOT_PATH "root"
#define DIR_UP ".."
#define DIR_ACT "."

#define MAX_PATH_LENGTH 1024
#define MAX_NAME_LENGTH 256
#define MAX_SHORT_NAME_LENGTH 64
#define MAX_MOUNT_POINT_LENGTH 2

// Max entries
#define MAX_POSITION 27
#define MAX_ENTRIES 28
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


#define NUM_DEVICES (sizeof(mydevices) / sizeof(char **))

typedef struct OrbisNfsBrowserDataTime
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	int microsecond;
}OrbisNfsBrowserDataTime;

typedef struct OrbisNfsBrowserDirEntry
{
	int namelen;
	unsigned char type;			/* entry type */
	unsigned char customtype;
	int mode;
	unsigned int size;			/* file size, in bytes */
	OrbisNfsBrowserDataTime atime;	/* time of last access */
	OrbisNfsBrowserDataTime mtime;	/* time of last data modification */
	OrbisNfsBrowserDataTime ctime;	/* time of last file status change */
	char name[MAX_PATH_LENGTH];
}OrbisNfsBrowserDirEntry;

typedef struct OrbisNfsBrowserListEntry {
	struct OrbisNfsBrowserListEntry *next;
	struct OrbisNfsBrowserListEntry *previous;
	struct OrbisNfsBrowserDirEntry *dir;
} OrbisNfsBrowserListEntry;

typedef struct OrbisNfsBrowserList {
	OrbisNfsBrowserListEntry *head;
	OrbisNfsBrowserListEntry *tail;
	int length;
	char path[MAX_PATH_LENGTH];
} OrbisNfsBrowserList;

typedef struct ExtensionTypes {
	char *extension;
	int type;
} ExtensionTypes;

typedef struct OrbisNfsEntry
{
	int fd;
	struct nfsfh* fileHandle;
	struct nfsdir* dirHandle;
	int64_t	size;
	char path[MAX_NAME_LENGTH];
}OrbisNfsEntry;

typedef struct OrbisNfsConfig
{
	struct nfs_context *nfsHandle;
	struct nfs_url *urls;
	int readChunkSize;
	int writeChunkSize;
	int openDescriptors[ORBISNFS_MAX_OPEN_DESCRIPTORS];
	OrbisNfsEntry *openEntries[ORBISNFS_MAX_OPEN_DESCRIPTORS];
	OrbisNfsBrowserList *browserList;
	int basePos; 
	int relPos;
	int basePosList[MAX_DIR_LEVELS];
	int relPosList[MAX_DIR_LEVELS];
	int dirLevel;
	int orbisnfs_initialized;
}OrbisNfsConfig;





int orbisNfsOpen(const char *file, int flags, int mode);
int orbisNfsClose(int fd);
int orbisNfsRead(int fd, void *data, size_t size);
int orbisNfsWrite(int fd, const void *data, size_t size);
int orbisNfsLseek(int fd, int offset, int whence);
int orbisNfsDopen(const char *path);
int orbisNfsDclose(int dfd);
int orbisNfsDread(int dfd, OrbisNfsBrowserDirEntry *entry);
int orbisNfsMkdir(const char *path, int mode);
unsigned char * orbisNfsGetFileContent(const char *file);
int orbisNfsInit(const char *nfsurl);
void orbisNfsFinish();
int orbisNfsInitWithConf(OrbisNfsConfig *conf);
int orbisNfsSetConf(OrbisNfsConfig *conf);
OrbisNfsConfig *orbisNfsGetConf();
int orbisNfsCreateConf();
char * orbisNfsGetExportPath();
int orbisNfsBrowserGetDirLevel();
int orbisNfsBrowserGetBasePos();
int orbisNfsBrowserGetRelPos();
char * orbisNfsBrowserGetListPath();
int orbisNfsBrowserGetListLength();
void orbisNfsBrowserSetListPath(char *path);
void orbisNfsBrowserEntryDown();
void orbisNfsBrowserEntryUp();
int orbisNfsBrowserGetFileType(char *file);
void orbisNfsBrowserDirLevelUp(char *path);
void orbisNfsBrowserDirLevelDown();
void orbisNfsBrowserListClean();
int orbisNfsBrowserListRefresh();
OrbisNfsBrowserListEntry *orbisNfsBrowserListGetNthEntry(int n);
void orbisNfsBrowserListAddEntry(OrbisNfsBrowserListEntry *entry,int sort);
int orbisNfsBrowserGetDirectoryEntries(char *path);
int orbisNfsBrowserGetDevices();
int orbisNfsBrowserListGetEntries(char *path);
	
	

#ifdef __cplusplus
}
#endif

#endif