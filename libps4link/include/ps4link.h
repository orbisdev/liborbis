#pragma once

#include <kernel.h>
#include <dirent.h>
#include <debugnet.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ps4LinkConfiguration
{
	int ps4link_fileio_active; 
	int ps4link_cmdsio_active;
	
	int ps4link_requests_port;
	int ps4link_commands_port;
	int ps4link_debug_port;
		
	int ps4link_fileio_sock;
	
	int ps4link_initialized;
	
	int ps4link_commands_sock;
	int ps4link_requests_sock;
	
	debugNetConfiguration *debugconf;
	
	
	
} ps4LinkConfiguration;
typedef enum ps4LinkValue
{
	FILEIO_ACTIVE=1,
	CMDSIO_ACTIVE=2,
	DEBUGNET_ACTIVE=3,
	PS4LINK_ACTIVE=4,	
	REQUESTS_PORT=5,
	COMMANDS_PORT=6,
	DEBUG_PORT=7,
	FILEIO_SOCK=8,
	REQUESTS_SOCK=9,
	COMMANDS_SOCK=10,
	DEBUG_SOCK=11,
	LOG_LEVEL=12,
} ps4LinkValue; 

typedef struct OrbisDataTime
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	int microsecond;
}OrbisDataTime;
typedef struct OrbisDirEntry
{
	int namelen;
	unsigned char type;			/* entry type */
	unsigned char customtype;
	int mode;
	unsigned int size;			/* file size, in bytes */
	OrbisDataTime atime;	/* time of last access */
	OrbisDataTime mtime;	/* time of last data modification */
	OrbisDataTime ctime;	/* time of last file status change */
	char name[256];
}OrbisDirEntry;


int ps4LinkOpen(const char *file, int flags, int mode);
int ps4LinkClose(int fd);
int ps4LinkRead(int fd, void *data, size_t size);
int ps4LinkWrite(int fd, const void *data, size_t size);
int ps4LinkLseek(int fd, int offset, int whence);
int ps4LinkRemove(const char *file);
int ps4LinkMkdir(const char *dirname, int mode);
int ps4LinkRmdir(const char *dirname);
int ps4LinkDopen(const char *dirname);
int ps4LinkDread(int fd, OrbisDirEntry *dir);
int ps4LinkDclose(int fd);
int ps4LinkConnect();




void ps4LinkRequestsAbort();
int ps4LinIsFinished();
int ps4LinkRequestsIsConnected();
int ps4LinkGetValue(ps4LinkValue val);
ps4LinkConfiguration *ps4LinkGetConfig();
int ps4LinkInitWithConf(ps4LinkConfiguration *conf);
int ps4LinkCreateConf();
int ps4LinkInit(char *serverIp, int requestPort,int debugPort, int commandPort, int level);
void ps4LinkFinish();

#ifdef __cplusplus
}
#endif
