/*
 *	debugnet library for PlayStation 4 
 *	Copyright (C) 2010,2016 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 *  Repository https://github.com/orbisdev/liborbis
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <net.h>
#include <sys/socket.h>

#include "debugnet.h"


int debugnet_external_conf=0;
debugNetConfiguration *dconfig=NULL;

/*static int debugnet_initialized=0;
int SocketFD = -1;
int logLevel=INFO;*/

/**
 * UDP printf for debugnet library 
 *
 * @par Example:
 * @code
 * debugNetUDPPrintf("This is a test\n");
 * @endcode
 */
void debugNetUDPPrintf(const char* fmt, ...)
{
  char buffer[0x800];
  va_list arg;
  va_start(arg, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, arg);
  va_end(arg);
  sceNetSend(dconfig->SocketFD, buffer, strlen(buffer), 0);
}
/**
 * Log Level printf for debugnet library 
 *
 * @par Example:
 * @code
 * debugNetPrintf(INFO,"This is a test\n");
 * @endcode
 *
 * @param level - NONE,INFO,ERROR or DEBUG
 */
void debugNetPrintf(int level, char* format, ...) 
{
	char msgbuf[0x800];
	va_list args;
	
		if (level>dconfig->logLevel)
		return;
       
	va_start(args, format);
	vsnprintf(msgbuf,2048, format, args);	
	msgbuf[2047] = 0;
	va_end(args);
	switch(level)
	{
		case INFO:
	    	debugNetUDPPrintf("[PS4][INFO]: %s",msgbuf);  
	        break;
	   	case ERROR: 
	    	debugNetUDPPrintf("[PS4][ERROR]: %s",msgbuf);
	        break;
		case DEBUG:
	        debugNetUDPPrintf("[PS4][DEBUG]: %s",msgbuf);
	        break;
		case NONE:
			break;
	    default:
		    debugNetUDPPrintf("%s",msgbuf);
       
	}
}
/**
 * Set log level for debugnet library 
 *
 * @par Example:
 * @code
 * debugNetSetLogLevel(DEBUG);  
 * @endcode
 * @param level - DEBUG,ERROR,INFO or NONE 
 */
void debugNetSetLogLevel(int level)
{
	if(dconfig)
	{
		dconfig->logLevel=level;	
	}
}
/**
 * Init debugnet library 
 *
 * @par Example:
 * @code
 * #define LOGLEVEL 3  
 * int ret;
 * ret = debugNetInit("172.26.0.2", 18194, DEBUG);
 * @endcode
 *
 * @param serverIP - your pc/mac server ip
 * @param port - udp port server
 * @param level - DEBUG,ERROR,INFO or NONE 
 */
int debugNetInit(char *serverIp, int port, int level)
{
	
	
   // struct SceNetSockaddrIn stSockAddr; SceNet data types not yet in libps3
	struct sockaddr_in stSockAddr;
	if(debugNetCreateConf())
	{
		return dconfig->debugnet_initialized;
	}
	
	debugNetSetLogLevel(level);
    
    
    //network initialization code is not needed on PS4 is started at boot
	
	/* Create datagram udp socket*/
    dconfig->SocketFD = sceNetSocket("debugnet_socket",AF_INET , SOCK_DGRAM, IPPROTO_UDP);
   
    memset(&stSockAddr, 0, sizeof stSockAddr);
	
	/*Populate SceNetSockaddrIn structure values*/
	
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = sceNetHtons(port);
	sceNetInetPton(AF_INET, serverIp, &stSockAddr.sin_addr);

	/*Connect socket to server*/
    sceNetConnect(dconfig->SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr);

	/*Show log on pc/mac side*/
	debugNetPrintf(INFO,"debugnet initialized\n");
	debugNetPrintf(INFO,"Copyright (C) 2010,2016 Antonio Jose Ramos Marquez aka bigboss @psxdev\n");
	debugNetPrintf(INFO,"ready to have a lot of fun...\n");

	/*library debugnet initialized*/
    dconfig->debugnet_initialized = 1;

    return dconfig->debugnet_initialized;
}
debugNetConfiguration *debugNetGetConf()
{	
	if(dconfig)
	{
		return dconfig;
	}
	
	return NULL;	
}
int debugNetSetConf(debugNetConfiguration *conf)
{	
	if(conf)
	{
		dconfig=conf;
		debugnet_external_conf=1;
		return dconfig->debugnet_initialized;
	}
	
	return 0;	
}
int debugNetInitWithConf(debugNetConfiguration *conf)
{
	int ret;
	ret=debugNetSetConf(conf);
	if(ret)
	{
		debugNetPrintf(INFO,"debugnet already initialized using configuration from ps4link\n");
		debugNetPrintf(INFO,"debugnet_initialized=%d SocketFD=%d logLevel=%d\n",dconfig->debugnet_initialized,dconfig->SocketFD,dconfig->logLevel);
		debugNetPrintf(INFO,"ready to have a lot of fun...\n");
		return dconfig->debugnet_initialized;
	}
	else
	{
		return 0;
	}
	
}
int debugNetCreateConf()
{	
	if(!dconfig)
	{
		dconfig=malloc(sizeof(debugNetConfiguration));
		dconfig->debugnet_initialized=0;
		dconfig->SocketFD = -1;
		dconfig->logLevel=INFO;	
		return 0;
	}
	
	if(dconfig->debugnet_initialized)
	{
		return 1;
	}
	return 0;			
}


/**
 * Finish debugnet library 
 *
 * @par Example:
 * @code
 * debugNetFinish();
 * @endcode
 */
void debugNetFinish()
{
	if(!debugnet_external_conf)
	{
    	if (dconfig->debugnet_initialized) {
        	dconfig->debugnet_initialized = 0;
			dconfig->SocketFD=-1;
    	}
	}
}
