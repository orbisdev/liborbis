/*
 *	debugnet library for PlayStation 4 
 *	Copyright (C) 2010,2016 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 *  Repository https://github.com/orbisdev/liborbis
 */
#ifndef _DEBUGNET_H_
#define _DEBUGNET_H_


#define NONE 0
#define INFO 1
#define ERROR 2
#define DEBUG 3	

typedef struct debugNetConfiguration
{
	int debugnet_initialized;
	int SocketFD;
	int logLevel;
	
} debugNetConfiguration;

int debugNetInit(char *serverIp, int port, int level);
int debugNetInitWithConf(debugNetConfiguration *conf);
debugNetConfiguration *debugNetGetConf();
int debugNetSetConf(debugNetConfiguration *conf);
void debugNetFinish();
void debugNetPrintf(int level, char* format, ...);
void debugNetSetLogLevel(int level);
int debugNetCreateConf();


#endif
