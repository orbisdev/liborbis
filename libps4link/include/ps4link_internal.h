/*
 * PS4link library for PlayStation 4 to communicate and use host file system with ps4sh host tool 
 * Copyright (C) 2003,2015,2016 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/ps4link
 * based on ps2vfs, ps2client, ps2link, ps2http tools. 
 * Credits goes for all people involved in ps2dev project https://github.com/ps2dev
 * This file is subject to the terms and conditions of the PS4Link License.
 * See the file LICENSE in the main directory of this distribution for more
 * details.
 */

#ifndef _PS4LINK_INTERNAL_H_
#define _PS4LINK_INTERNAL_H_

#define PS4LINK_OPEN_CMD     0xbabe0111
#define PS4LINK_OPEN_RLY     0xbabe0112
#define PS4LINK_CLOSE_CMD    0xbabe0121
#define PS4LINK_CLOSE_RLY    0xbabe0122
#define PS4LINK_READ_CMD     0xbabe0131
#define PS4LINK_READ_RLY     0xbabe0132
#define PS4LINK_WRITE_CMD    0xbabe0141
#define PS4LINK_WRITE_RLY    0xbabe0142
#define PS4LINK_LSEEK_CMD    0xbabe0151
#define PS4LINK_LSEEK_RLY    0xbabe0152
#define PS4LINK_OPENDIR_CMD  0xbabe0161
#define PS4LINK_OPENDIR_RLY  0xbabe0162
#define PS4LINK_CLOSEDIR_CMD 0xbabe0171
#define PS4LINK_CLOSEDIR_RLY 0xbabe0172
#define PS4LINK_READDIR_CMD  0xbabe0181
#define PS4LINK_READDIR_RLY  0xbabe0182
#define PS4LINK_REMOVE_CMD   0xbabe0191
#define PS4LINK_REMOVE_RLY   0xbabe0192
#define PS4LINK_MKDIR_CMD    0xbabe01a1
#define PS4LINK_MKDIR_RLY    0xbabe01a2
#define PS4LINK_RMDIR_CMD    0xbabe01b1
#define PS4LINK_RMDIR_RLY    0xbabe01b2
#define PS4LINK_CONN_CMD	 0xbabe01c1
#define PS4LINK_CONN_RLY	 0xbabe01c2



#define PS4LINK_MAX_PATH   256


typedef struct
{
    unsigned int cmd;
    unsigned short len;
} __attribute__((packed)) ps4link_pkt_hdr;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    unsigned int retval;
} __attribute__((packed)) ps4link_pkt_file_rly;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
} __attribute__((packed)) ps4link_pkt_conn_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    unsigned int retval;
} __attribute__((packed)) ps4link_pkt_conn_rly;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int flags;
    char path[PS4LINK_MAX_PATH];
} __attribute__((packed)) ps4link_pkt_open_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int fd;
} __attribute__((packed)) ps4link_pkt_close_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int fd;
    int nbytes;
} __attribute__((packed)) ps4link_pkt_read_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int retval;
    int nbytes;
} __attribute__((packed)) ps4link_pkt_read_rly;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int fd;
    int nbytes;
} __attribute__((packed)) ps4link_pkt_write_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int fd;
	int offset;
    int whence;
} __attribute__((packed)) ps4link_pkt_lseek_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    char name[PS4LINK_MAX_PATH];
} __attribute__((packed)) ps4link_pkt_remove_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int mode;
    char name[PS4LINK_MAX_PATH];
} __attribute__((packed)) ps4link_pkt_mkdir_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    char name[PS4LINK_MAX_PATH];
} __attribute__((packed)) ps4link_pkt_rmdir_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int fd;
} __attribute__((packed)) ps4link_pkt_dread_req;

typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int retval;
	unsigned char type;
	int mode;
	unsigned int size;
	unsigned short ctime[8];
	unsigned short atime[8];
	unsigned short mtime[8];
	char name[256];
} __attribute__((packed)) ps4link_pkt_dread_rly;

#define PS4LINK_EXECUSER_CMD 0xbabe0201
#define	PS4LINK_EXECKERNEL_CMD 0xbabe0202
#define	PS4LINK_EXIT_CMD 0xbabe0203
#define	PS4LINK_EXECDECRYPT_CMD 0xbabe0204
#define	PS4LINK_EXECWHOAMI_CMD 0xbabe0205
#define	PS4LINK_EXECSHOWDIR_CMD 0xbabe0206
#define	PS4LINK_EXECPTRACE_CMD 0xbabe0207




typedef struct
{
    unsigned int cmd;
    unsigned short len;
    int  argc;
    char argv[PS4LINK_MAX_PATH];
} __attribute__((packed)) ps4link_pkt_exec_cmd;

//1460 ORIG
#define PS4LINK_MAX_WRITE_SEGMENT (32768 - sizeof(ps4link_pkt_write_req))  //1460
#define PS4LINK_MAX_READ_SEGMENT  (32768 - sizeof(ps4link_pkt_read_rly)) //1460

void *ps4link_requests_thread(void *args);
void *ps4link_commands_thread(void *args);

#endif
