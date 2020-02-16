/*
 * PS4link library for PlayStation 4 to communicate and use host file system with ps4sh host tool 
 * Copyright (C) 2003,2015,2016 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 * based on ps2vfs, ps2client, ps2link, ps2http tools. 
 * Credits goes for all people involved in ps2dev project https://github.com/ps2dev
 * This file is subject to the terms and conditions of the PS4Link License.
 * See the file LICENSE in the main directory of this distribution for more
 * details.
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <unistd.h>
#include <net.h>
#include <sys/socket.h>
#include <debugnet.h>



#include "ps4link_internal.h"
#include "ps4link.h"
#include "jailbreak.h"


/*int ps4link_initialized;
int ps4link_requests_port;
int ps4link_commands_port;
int ps4link_fileio_active; 
int ps4link_cmdio_active;*/

ScePthread server_request_thid;
ScePthread server_command_thid;
//ScePthread server_payload_thid;

ps4LinkConfiguration *configuration=NULL;
int ps4link_exit=0;
int external_conf=0;

extern int ps4KernelExecute(void *fn, void *uap, int64_t *ret0, int64_t *ret1);
/**
 * Init ps4link library 
 *
 * @par Example:
 * @code
 * int ret;
 * ret = psp2LinkInit("172.26.0.2",0x4711,0x4712,0x4712, DEBUG);
 * @endcode
 * 
 * @param serverIp - server ip for udp debug
 * @param requestPort - ps4  port server for fileio requests
 * @param debugPort - udp port for debug
 * @param commandPort - ps4  port server for commands
 * @param level - DEBUG,ERROR,INFO or NONE 
 */
int ps4LinkInit(char *serverIp, int requestPort,int debugPort, int commandPort,int level)
{
	int ret;
    int64_t ret1;
	
	if(ps4LinkCreateConf())
	{
		return ps4LinkGetValue(PS4LINK_ACTIVE);
	}
	configuration->ps4link_requests_port=requestPort;
	configuration->ps4link_commands_port=commandPort;
	configuration->ps4link_debug_port=commandPort;
	
	
	if(debugNetInit(serverIp,debugPort,level))
	{
		sleep(2);
		
		configuration->debugconf=debugNetGetConf();
		
		
		/*	ret=scePthreadCreate(&server_payload_thid, NULL, payload_thread, NULL, "ps4link_request_server_thread");
			if(ret==0)
			{
				debugNetPrintf(DEBUG,"[PS4LINK] Server payload thread UID: 0x%08X\n", server_payload_thid);
			
			}
			else
			{
				debugNetPrintf(DEBUG,"[PS4LINK] Server payload thread could not create error: 0x%08X\n", ret);
				scePthreadCancel(server_payload_thid);
			
			
			}*/
		
		
		//scePthreadJoin(server_payload_thid, NULL);
	  //  debugNetPrintf(DEBUG,"getuid() : %d\n", getuid());
	  // 	if (getuid() != 0) {
	  //	   debugNetPrintf(DEBUG,"getuid : %d getgid=%d\n", getuid(),getgid());
		
		   //debugNetPrintf(DEBUG,"executing privilege scalation \n");
			
			
			
		   //myJailbreak();
			
		   // debugNetPrintf(DEBUG,"getuid() : %d\n", getuid());
		debugNetPrintf(DEBUG,"getuid : %d getgid=%d\n",getuid(),getgid());
			
		
		    
	    //}
		
		ret=scePthreadCreate(&server_request_thid, NULL, ps4link_requests_thread, NULL, "ps4link_request_server_thread");
		

		if(ret==0)
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server request thread UID: 0x%08X\n", server_request_thid);
			
		}
		else
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server request thread could not create error: 0x%08X\n", ret);
			scePthreadCancel(server_request_thid);
			ps4LinkFinish();
			return 0;
		}
	
		ret=scePthreadCreate(&server_command_thid, NULL, ps4link_commands_thread, NULL, "ps4link_command_server_thread");
		
		if(ret==0)
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server command thread UID: 0x%08X\n", server_command_thid);
		}
		else
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server command thread could not create error: 0x%08X\n", server_command_thid);
			scePthreadCancel(server_request_thid);
			scePthreadCancel(server_command_thid);
			ps4LinkFinish();
			return 0;
		}
	
		/*ret=scePthreadCancel(server_payload_thid);
		if(ret==0)
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server payload thread terminated\n");
		}
		else
		{
			debugNetPrintf(DEBUG,"[PS4LINK] Server payload thread terminated error: 0x%08X\n", ret);
			
		}*/
	
		/*ps4link library initialized*/	
	    configuration->ps4link_initialized = 1;
		
	}
	else
	{
		configuration->ps4link_initialized = 0;
	}
	sleep(2);
    return ps4LinkGetValue(PS4LINK_ACTIVE);
}
int ps4LinkSetConfig(ps4LinkConfiguration *conf)
{
	if(!conf)
	{
		return 0;
	}
	configuration=conf;
	external_conf=1;
	return 1;
}
ps4LinkConfiguration *ps4LinkGetConfig()
{
	if(!configuration)
	{
		return NULL;
	}
	return configuration;
}
int ps4LinkInitWithConf(ps4LinkConfiguration *conf)
{
	
	int ret;
	ret=ps4LinkSetConfig(conf);
	if(ret)
	{
		if(debugNetInitWithConf(conf->debugconf))
		{
		
			debugNetPrintf(INFO,"ps4link already initialized using configuration from ps4link\n");			
			debugNetPrintf(INFO,"ps4link_fileio_active = %d\n",ps4LinkGetValue(FILEIO_ACTIVE));
			debugNetPrintf(INFO,"ps4link_cmdsio_active = %d\n",ps4LinkGetValue(CMDSIO_ACTIVE));
			debugNetPrintf(INFO,"ps4link_initialized   = %d\n",ps4LinkGetValue(PS4LINK_ACTIVE));
			debugNetPrintf(INFO,"ps4link_requests_port = %d\n",ps4LinkGetValue(REQUESTS_PORT));
			debugNetPrintf(INFO,"ps4link_commands_port = %d\n",ps4LinkGetValue(COMMANDS_PORT));
			debugNetPrintf(INFO,"ps4link_debug_port    = %d\n",ps4LinkGetValue(DEBUG_PORT));
			debugNetPrintf(INFO,"ps4link_fileio_sock   = %d\n",ps4LinkGetValue(FILEIO_SOCK));
			debugNetPrintf(INFO,"ps4link_requests_sock = %d\n",ps4LinkGetValue(REQUESTS_SOCK));
			debugNetPrintf(INFO,"ps4link_commands_sock = %d\n",ps4LinkGetValue(COMMANDS_SOCK));
			return ps4LinkGetValue(PS4LINK_ACTIVE);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int ps4LinkCreateConf()
{	
	if(!configuration)
	{
		configuration=malloc(sizeof(ps4LinkConfiguration));
		configuration->ps4link_fileio_active=1;
		configuration->ps4link_cmdsio_active=1;
		configuration->ps4link_initialized=0;
		configuration->ps4link_fileio_sock=-1;
		configuration->ps4link_requests_sock=-1;
		configuration->ps4link_commands_sock=-1;
		return 0;
	}
	if(configuration->ps4link_initialized)
	{
		return 1;
	}
	return 0;			
}


/**
 * Get configuration values from PS4Link library 
 *
 * @par Example:
 * @code
 * ps4LinkGetValue(PS4LINK_ACTIVE);
 * @endcode
 */
int ps4LinkGetValue(ps4LinkValue val)
{
	int ret;
	switch(val)
	{
		case FILEIO_ACTIVE:
			ret=configuration->ps4link_fileio_active;
			break;
		case CMDSIO_ACTIVE:
			ret=configuration->ps4link_cmdsio_active;
			break;
		case DEBUGNET_ACTIVE:
			ret=configuration->debugconf->debugnet_initialized;
			break;
		case PS4LINK_ACTIVE:
			ret=configuration->ps4link_initialized;
			break;
		case REQUESTS_PORT:
			ret=configuration->ps4link_requests_port;
			break;
		case COMMANDS_PORT:
			ret=configuration->ps4link_commands_port;
			break;
		case DEBUG_PORT:
			ret=configuration->ps4link_debug_port;
			break;
		case FILEIO_SOCK:
			ret=configuration->ps4link_fileio_sock;
			break;
		case REQUESTS_SOCK:
			ret=configuration->ps4link_requests_sock;
			break;
		case COMMANDS_SOCK:
			ret=configuration->ps4link_commands_sock;
			break;
		case DEBUG_SOCK:
			ret=configuration->debugconf->SocketFD;
			break;
		case LOG_LEVEL:
			ret=configuration->debugconf->logLevel;
			break;
		default:
			ret=0;
			break;
	}
	return ret;
}
int ps4LinkIsFinished()
{
	return ps4link_exit;
}
/**
 * Finish PS4Link library 
 *
 * @par Example:
 * @code
 * ps4LinkFinish();
 * @endcode
 */
void ps4LinkFinish()
{
	if(!external_conf
	&& configuration)
	{
		configuration->ps4link_fileio_active=0;
		configuration->ps4link_cmdsio_active=0;
    	configuration->ps4link_initialized=0;
		ps4LinkRequestsAbort();
		ps4link_exit=1;
		
		while(ps4LinkRequestsIsConnected())
		{
		
		}
		debugNetFinish();
	}
}
