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

#include <debugnet.h>
#include <net.h>
#include <sys/socket.h>
#include <string.h>

#include "ps4link.h"
#include "ps4link_internal.h"

//4096 ORIG
#define PACKET_MAXSIZE 32768

static char send_packet[PACKET_MAXSIZE] __attribute__((aligned(16)));
static char recv_packet[PACKET_MAXSIZE] __attribute__((aligned(16)));
unsigned int remote_pc_addr;
extern ps4LinkConfiguration *configuration;


int ps4link_requests_connected;

int ps4LinkRequestsIsConnected()
{
 	if(ps4link_requests_connected)
 	{
		//debugNetPrintf(DEBUG,"[PS4LINK] ps4link connected  %d\n", ps4link_requests_connected);
 	}
	else
	{
       // DEBUG(INFO,"ps4link is not connected   %d\n", no_connected);
		
		
	}
	return ps4link_requests_connected;
}
void ps4link_close_socket(void)
{
	int ret;

	ret = sceNetSocketClose(ps4LinkGetValue(FILEIO_SOCK));
	if (ret < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] close ps4link_fileio_sock returned error 0x%08X\n", ret);
	}
	configuration->ps4link_fileio_sock = -1;
	
}


void ps4LinkRequestsAbort()
{
	int ret;
	if(ps4LinkGetValue(REQUESTS_SOCK))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] Aborting ps4link_requests_sock\n");
		
		ret = sceNetSocketAbort(ps4LinkGetValue(REQUESTS_SOCK),1);
		if (ret < 0) {
			debugNetPrintf(DEBUG,"[PS4LINK] abort ps4link_requests_sock returned error 0x%08X\n", ret);
		}
	}
	
}
static inline int ps4link_send(int sock, void *buf, int len, int flag)
{
	int ret;
	ret = sceNetSend(sock, buf, len, flag);
	if (ret < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] sceNetSend error %d\n", ret);
		ps4link_close_socket();
		return -1;
	}
	else {
		return ret;
	}
}

int ps4link_recv_bytes(int sock, char *buf, int bytes)
{
	size_t left;
	int len;

	left =(size_t) bytes;

	while (left > 0) {
		len = sceNetRecv(sock, &buf[bytes - left], left, MSG_WAITALL);
		//len = read(sock, &buf[bytes - left], left);
		
		if (len < 0) {
			debugNetPrintf(DEBUG,"[PS4LINK] ps4link_recv_bytes error!! 0x%08X\n",len);
			return -1;
		}
		left -= len;
	}
	return bytes;
	
	
	
	
	
}


int ps4link_accept_pkt(int sock, char *buf, int len, int pkt_type)
{
	int length;
	ps4link_pkt_hdr *hdr;
	unsigned int hcmd;
	unsigned short hlen;


	length = ps4link_recv_bytes(sock, buf, sizeof(ps4link_pkt_hdr));
	if (length < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_accept_pkt recv error\n");
		return -1;
	}

	if (length < sizeof(ps4link_pkt_hdr)) {
		debugNetPrintf(DEBUG,"[PS4LINK] did not receive a full header!!!! " "Fix this! (%d)\n", length);
	}
    
	hdr = (ps4link_pkt_hdr *)buf;
	hcmd = sceNetNtohl(hdr->cmd);
	hlen = sceNetNtohs(hdr->len);

	if (hcmd != pkt_type) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_accept_pkt: Expected %x, got %x\n",pkt_type, hcmd);
		return 0;
	}

	if (hlen > len) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_accept_pkt: hdr->len is too large!! " "(%d, can only receive %d)\n", hlen, len);
		return 0;
	}

	// get the actual packet data
	length = ps4link_recv_bytes(sock, buf + sizeof(ps4link_pkt_hdr),hlen - sizeof(ps4link_pkt_hdr));

	if (length < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] accept recv2 error!!\n");
		return 0;
	}

	if (length < (hlen - sizeof(ps4link_pkt_hdr))) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_accept_pkt: Did not receive full packet!!! " ,"Fix this! (%d)\n", length);
	}

	return 1;
}
int ps4LinkConnect()
{
	ps4link_pkt_conn_req *connreq;
	ps4link_pkt_conn_rly *connrly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_fileio_sock is not active\n");
		
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] conn req\n");

	connreq = (ps4link_pkt_conn_req *)&send_packet[0];

	// Build packet
	connreq->cmd = sceNetHtonl(PS4LINK_CONN_CMD);
	connreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_conn_req));

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK),connreq, sizeof(ps4link_pkt_conn_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_conn_rly), PS4LINK_CONN_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: ps4LinkRequestConnect: did not receive CONN_RLY\n");
		return -1;
	}

	connrly = (ps4link_pkt_conn_rly *)recv_packet;
    
	debugNetPrintf(DEBUG,"[PS4LINK] file conn reply received (ret %d)\n", sceNetNtohl(connrly->retval));

	return sceNetNtohl(connrly->retval);	
	
	
}

int ps4LinkOpen(const char *file, int flags, int mode)
{
	ps4link_pkt_open_req *openreq;
	ps4link_pkt_file_rly *openrly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] file open req (%s, %x) but ps4link_fileio_sock is not active\n", file, flags);
		
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] file open req (%s, %x %x)\n", file, flags, mode);

	openreq = (ps4link_pkt_open_req *)&send_packet[0];

	// Build packet
	openreq->cmd = sceNetHtonl(PS4LINK_OPEN_CMD);
	openreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_open_req));
	openreq->flags = sceNetHtonl(flags);
	strncpy(openreq->path, file, PS4LINK_MAX_PATH);
	openreq->path[PS4LINK_MAX_PATH - 1] = 0; // Make sure it's null-terminated

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), openreq, sizeof(ps4link_pkt_open_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_file_rly), PS4LINK_OPEN_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: ps4link_open_file: did not receive OPEN_RLY\n");
		return -1;
	}

	openrly = (ps4link_pkt_file_rly *)recv_packet;
    
	debugNetPrintf(DEBUG,"[PS4LINK] file open reply received (ret %d)\n", sceNetNtohl(openrly->retval));

	return sceNetNtohl(openrly->retval);	
	
	
}
int ps4LinkClose(int fd)
{
	
	ps4link_pkt_close_req *closereq;
	ps4link_pkt_file_rly *closerly;


	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: file close req (fd: %d)\n", fd);

	closereq = (ps4link_pkt_close_req *)&send_packet[0];
	closerly = (ps4link_pkt_file_rly *)&recv_packet[0];

	closereq->cmd = sceNetHtonl(PS4LINK_CLOSE_CMD);
	closereq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_close_req));
	closereq->fd = sceNetHtonl(fd);

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), closereq, sizeof(ps4link_pkt_close_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if(!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), (char *)closerly, sizeof(ps4link_pkt_file_rly), PS4LINK_CLOSE_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_close_file: did not receive PS4LINK_CLOSE_RLY\n");
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_close_file: close reply received (ret %d)\n", sceNetNtohl(closerly->retval));

	return sceNetNtohl(closerly->retval);
	
	
	
}
int ps4LinkRead(int fd, void *data, size_t size)
{
	int readbytes;
	int nbytes;
	int i;
	int j;
	int numread;
	int lastread;
	ps4link_pkt_read_req *readcmd;
	ps4link_pkt_read_rly *readrly;


	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	readcmd = (ps4link_pkt_read_req *)&send_packet[0];
	readrly = (ps4link_pkt_read_rly *)&recv_packet[0];
	readbytes = 0;

	readcmd->cmd = sceNetHtonl(PS4LINK_READ_CMD);
	readcmd->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_read_req));
	readcmd->fd = sceNetHtonl(fd);

	readbytes = 0;

	/*if (size < 0) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file: illegal req!! (whish to read < 0 bytes!)\n");
		return -1;
	}*/

	readcmd->nbytes = sceNetHtonl(size);

	i = ps4link_send(ps4LinkGetValue(FILEIO_SOCK), readcmd, sizeof(ps4link_pkt_read_req), MSG_DONTWAIT);
	if (i<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: ps4link_read_file: send failed (%d)\n", i);
		return -1;
	}

	if(!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), (char *)readrly, sizeof(ps4link_pkt_read_rly), PS4LINK_READ_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file: " "did not receive PS4LINK_READ_RLY\n");
		return -1;
	}

	nbytes = sceNetNtohl(readrly->nbytes);
	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file: Reply said there's %d bytes to read " "(wanted %d)\n", nbytes, size);

	// Now read the actual file data divide in chunk
	if(nbytes<PACKET_MAXSIZE)
	{
		i=ps4link_recv_bytes(ps4LinkGetValue(FILEIO_SOCK), data, nbytes);
		if (i < 0) {
	    	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file, data read error\n");
	    	return -1;
		}
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file: readed %d\n",i);
		
	}
	else
	{
		numread=nbytes/PACKET_MAXSIZE;
		lastread=nbytes%PACKET_MAXSIZE;
		for(j=0;j<numread;j++)
		{
			if(j<numread-1)
			{
				i = ps4link_recv_bytes(ps4LinkGetValue(FILEIO_SOCK), data+j*PACKET_MAXSIZE, PACKET_MAXSIZE);
			}
			else
			{
				i = ps4link_recv_bytes(ps4LinkGetValue(FILEIO_SOCK), data+j*PACKET_MAXSIZE, PACKET_MAXSIZE+lastread);
			
			}
			if (i < 0) {
	    		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file, data read error\n");
	    		return -1;
			}
			debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_file: chunk %d  readed %d\n", j,i);
		}
		
	}
	
	
	return nbytes;
}
int ps4LinkWrite(int fd, const void *data, size_t size)
{
	ps4link_pkt_write_req *writecmd;
	ps4link_pkt_file_rly *writerly;
	int hlen;
	int writtenbytes;
	int nbytes;
	int retval;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] file write req (fd: %d)\n", fd);

	writecmd = (ps4link_pkt_write_req *)&send_packet[0];
	writerly = (ps4link_pkt_file_rly *)&recv_packet[0];

	hlen = (unsigned short)sizeof(ps4link_pkt_write_req);
	writecmd->cmd = sceNetHtonl(PS4LINK_WRITE_CMD);
	writecmd->len = sceNetHtons(hlen);
	writecmd->fd = sceNetHtonl(fd);

	// Divide the write request
	writtenbytes = 0;
	while (writtenbytes < size) {

		if ((size - writtenbytes) > PS4LINK_MAX_WRITE_SEGMENT) {
			// Need to split in several read reqs
			nbytes = PS4LINK_MAX_READ_SEGMENT;
        }
		else 
		{
			nbytes = size - writtenbytes;
		}

		writecmd->nbytes = sceNetHtonl(nbytes);
#ifdef ZEROCOPY
		/* Send the packet header.  */
		if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), writecmd, hlen, MSG_DONTWAIT) < 0)
			return -1;
		/* Send the write() data.  */
		if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), &data[writtenbytes], nbytes, MSG_DONTWAIT) < 0)
			return -1;
#else
		// Copy data to the acutal packet
		memcpy(&send_packet[sizeof(ps4link_pkt_write_req)], &data[writtenbytes],nbytes);

		if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), writecmd, hlen + nbytes, MSG_DONTWAIT) < 0)
			return -1;
#endif


		// Get reply
		if(!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), (char *)writerly, sizeof(ps4link_pkt_file_rly), PS4LINK_WRITE_RLY)) {
			debugNetPrintf(DEBUG,"[PS4LINK] ps4link_write_file: " "did not receive PS4LINK_WRITE_RLY\n");
			return -1;
		}
		retval = sceNetNtohl(writerly->retval);

		debugNetPrintf(DEBUG,"[PS4LINK] wrote %d bytes (asked for %d)\n", retval, nbytes);

		if (retval < 0) {
			// Error
			debugNetPrintf(DEBUG,"[PS4LINK] ps4link_write_file: received error on write req (%d)\n",retval);
			return retval;
		}

		writtenbytes += retval;
		if (retval < nbytes) {
			// EOF?
			break;
		}
	}
	
	return writtenbytes;	

	
}
int ps4LinkLseek(int fd, int offset, int whence)	
{
	ps4link_pkt_lseek_req *lseekreq;
	ps4link_pkt_file_rly *lseekrly;


	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] file lseek req (fd: %d)\n", fd);

	lseekreq = (ps4link_pkt_lseek_req *)&send_packet[0];
	lseekrly = (ps4link_pkt_file_rly *)&recv_packet[0];

	lseekreq->cmd = sceNetHtonl(PS4LINK_LSEEK_CMD);
	lseekreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_lseek_req));
	lseekreq->fd = sceNetHtonl(fd);
	lseekreq->offset = sceNetHtonl(offset);
	lseekreq->whence = sceNetHtonl(whence);

	if(ps4link_send(ps4LinkGetValue(FILEIO_SOCK), lseekreq, sizeof(ps4link_pkt_lseek_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if(!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), (char *)lseekrly,sizeof(ps4link_pkt_file_rly), PS4LINK_LSEEK_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_lseek_file: did not receive PS4LINK_LSEEK_RLY\n");
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_lseek_file: lseek reply received (ret %d)\n",sceNetNtohl(lseekrly->retval));

	return sceNetNtohl(lseekrly->retval);
	
}
int ps4LinkRemove(const char *file)
{
	ps4link_pkt_remove_req *removereq;
	ps4link_pkt_file_rly *removerly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] file remove req (%s)\n", file);

	removereq = (ps4link_pkt_remove_req *)&send_packet[0];

	// Build packet
	removereq->cmd = sceNetHtonl(PS4LINK_REMOVE_CMD);
	removereq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_remove_req));
	strncpy(removereq->name, file, PS4LINK_MAX_PATH);
	removereq->name[PS4LINK_MAX_PATH - 1] = 0; // Make sure it's null-terminated

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), removereq, sizeof(ps4link_pkt_remove_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_file_rly), PS4LINK_REMOVE_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_remove: did not receive REMOVE_RLY\n");
		return -1;
	}

	removerly = (ps4link_pkt_file_rly *)recv_packet;
	debugNetPrintf(DEBUG,"[PS4LINK] file remove reply received (ret %d)\n", sceNetNtohl(removerly->retval));
	return sceNetNtohl(removerly->retval);
}
int ps4LinkMkdir(const char *dirname, int mode)
{
	ps4link_pkt_mkdir_req *mkdirreq;
	ps4link_pkt_file_rly *mkdirrly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] make dir req (%s)\n", dirname);
	
	mkdirreq = (ps4link_pkt_mkdir_req *)&send_packet[0];

	// Build packet
	mkdirreq->cmd = sceNetHtonl(PS4LINK_MKDIR_CMD);
	mkdirreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_mkdir_req));
	mkdirreq->mode = mode;
	strncpy(mkdirreq->name, dirname, PS4LINK_MAX_PATH);
	mkdirreq->name[PS4LINK_MAX_PATH - 1] = 0; // Make sure it's null-terminated

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), mkdirreq, sizeof(ps4link_pkt_mkdir_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_file_rly), PS4LINK_MKDIR_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_mkdir: did not receive MKDIR_RLY\n");
		return -1;
	}

	mkdirrly = (ps4link_pkt_file_rly *)recv_packet;
	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: make dir reply received (ret %d)\n", sceNetNtohl(mkdirrly->retval));
	return sceNetNtohl(mkdirrly->retval);
}
int ps4LinkRmdir(const char *dirname)
{
	ps4link_pkt_rmdir_req *rmdirreq;
	ps4link_pkt_file_rly *rmdirrly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: remove dir req (%s)\n", dirname);

	rmdirreq = (ps4link_pkt_rmdir_req *)&send_packet[0];

	// Build packet
	rmdirreq->cmd = sceNetHtonl(PS4LINK_RMDIR_CMD);
	rmdirreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_rmdir_req));
	strncpy(rmdirreq->name, dirname, PS4LINK_MAX_PATH);
	rmdirreq->name[PS4LINK_MAX_PATH - 1] = 0; // Make sure it's null-terminated

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), rmdirreq, sizeof(ps4link_pkt_rmdir_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_file_rly), PS4LINK_RMDIR_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: ps4link_rmdir: did not receive RMDIR_RLY\n");
		return -1;
	}

    rmdirrly = (ps4link_pkt_file_rly *)recv_packet;
	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: remove dir reply received (ret %d)\n", sceNetNtohl(rmdirrly->retval));
	return sceNetNtohl(rmdirrly->retval);
}
int ps4LinkDopen(const char *dirname)
{
	ps4link_pkt_open_req *openreq;
	ps4link_pkt_file_rly *openrly;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] dir open req (%s)\n", dirname);

	openreq = (ps4link_pkt_open_req *)&send_packet[0];

	// Build packet
	openreq->cmd = sceNetHtonl(PS4LINK_OPENDIR_CMD);
	openreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_open_req));
 	openreq->flags = sceNetHtonl(0);
	strncpy(openreq->path, dirname, PS4LINK_MAX_PATH);
	openreq->path[PS4LINK_MAX_PATH - 1] = 0; // Make sure it's null-terminated

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), openreq, sizeof(ps4link_pkt_open_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_file_rly), PS4LINK_OPENDIR_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_open_dir: did not receive OPENDIR_RLY\n");
		return -1;
	}

	openrly = (ps4link_pkt_file_rly *)recv_packet;
    
	debugNetPrintf(DEBUG,"[PS4LINK] dir open reply received (ret %d)\n", sceNetNtohl(openrly->retval));

	return sceNetNtohl(openrly->retval);
}
int ps4LinkDread(int fd, OrbisDirEntry *dir)
{
	ps4link_pkt_dread_req *dirreq;
	ps4link_pkt_dread_rly *dirrly;
	OrbisDirEntry *dirent;

	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] dir read req (%x)\n", fd);
	if(fd>=0)
	{
	dirreq = (ps4link_pkt_dread_req *)&send_packet[0];

	// Build packet
	dirreq->cmd = sceNetHtonl(PS4LINK_READDIR_CMD);
	dirreq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_dread_req));
	dirreq->fd = sceNetHtonl(fd);

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), dirreq, sizeof(ps4link_pkt_dread_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if (!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), recv_packet, sizeof(ps4link_pkt_dread_rly), PS4LINK_READDIR_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_read_dir: did not receive OPENDIR_RLY\n");
		return -1;
	}

	dirrly = (ps4link_pkt_dread_rly *)recv_packet;
    
	debugNetPrintf(DEBUG,"[PS4LINK] dir read reply received (ret %d)\n", sceNetNtohl(dirrly->retval));

	
	dirent = dir;
	   
	strncpy(dirent->name,dirrly->name,strlen(dirrly->name)+1);
	//dirent->name[strlen(dirrly->name)+1]='\0';
	debugNetPrintf(DEBUG,"[PS4LINK]dirrly %s %d\n", dirrly->name,strlen(dirrly->name));
	debugNetPrintf(DEBUG,"[PS4LINK]dirent %s %d\n", dirent->name,strlen(dirent->name));
	
	dirent->type=dirrly->type;
	dirent->customtype=0;
	dirent->mode=sceNetNtohl(dirrly->mode);
	dirent->size=sceNetNtohl(dirrly->size);
	dirent->namelen=strlen(dirent->name);
	
	dirent->ctime.year=sceNetNtohs(dirrly->ctime[6]);
	dirent->ctime.month=sceNetNtohs(dirrly->ctime[5]);;
	dirent->ctime.day=sceNetNtohs(dirrly->ctime[4]);;
	dirent->ctime.hour=sceNetNtohs(dirrly->ctime[3]);;
	dirent->ctime.minute=sceNetNtohs(dirrly->ctime[2]);;
	dirent->ctime.second=sceNetNtohs(dirrly->ctime[1]);
	dirent->ctime.microsecond=0;
	
	dirent->atime.year=sceNetNtohs(dirrly->atime[6]);
	dirent->atime.month=sceNetNtohs(dirrly->atime[5]);;
	dirent->atime.day=sceNetNtohs(dirrly->atime[4]);;
	dirent->atime.hour=sceNetNtohs(dirrly->atime[3]);;
	dirent->atime.minute=sceNetNtohs(dirrly->atime[2]);;
	dirent->atime.second=sceNetNtohs(dirrly->atime[1]);
	dirent->atime.microsecond=0;
	
	dirent->mtime.year=sceNetNtohs(dirrly->mtime[6]);
	dirent->mtime.month=sceNetNtohs(dirrly->mtime[5]);;
	dirent->mtime.day=sceNetNtohs(dirrly->mtime[4]);;
	dirent->mtime.hour=sceNetNtohs(dirrly->mtime[3]);;
	dirent->mtime.minute=sceNetNtohs(dirrly->mtime[2]);;
	dirent->mtime.second=sceNetNtohs(dirrly->mtime[1]);
	dirent->mtime.microsecond=0;
	

	return sceNetNtohl(dirrly->retval);
	}
	return -1;
}
int ps4LinkDclose(int fd)	
{
 	ps4link_pkt_close_req *closereq;
	ps4link_pkt_file_rly *closerly;


	if (ps4LinkGetValue(FILEIO_SOCK) < 0) {
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] ps4link_file: dir close req (fd: %d)\n", fd);
	if(fd>=0)
	{
	closereq = (ps4link_pkt_close_req *)&send_packet[0];
	closerly = (ps4link_pkt_file_rly *)&recv_packet[0];

	closereq->cmd = sceNetHtonl(PS4LINK_CLOSEDIR_CMD);
	closereq->len = sceNetHtons((unsigned short)sizeof(ps4link_pkt_close_req));
	closereq->fd = sceNetHtonl(fd);

	if (ps4link_send(ps4LinkGetValue(FILEIO_SOCK), closereq, sizeof(ps4link_pkt_close_req), MSG_DONTWAIT) < 0) {
		return -1;
	}

	if(!ps4link_accept_pkt(ps4LinkGetValue(FILEIO_SOCK), (char *)closerly, sizeof(ps4link_pkt_file_rly), PS4LINK_CLOSEDIR_RLY)) {
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_close_dir: did not receive PS4LINK_CLOSEDIR_RLY\n");
		return -1;
	}

	debugNetPrintf(DEBUG,"[PS4LINK] dir close reply received (ret %d)\n", sceNetNtohl(closerly->retval));

	return sceNetNtohl(closerly->retval);
	}
	return -1;
}
void *ps4link_requests_thread(void * args)
{
	int ret;
	struct sockaddr_in serveraddr;
	ps4link_requests_connected=0;
	/* Create server socket */
	configuration->ps4link_requests_sock = sceNetSocket("requests_server_sock",AF_INET,SOCK_STREAM,0);
	if(ps4LinkGetValue(REQUESTS_SOCK)>=0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] Created ps4link_requests_sock: %d\n", ps4LinkGetValue(REQUESTS_SOCK));
		
	}
  	
	/* Fill the server's address */
	serveraddr.sin_len = 16;
	serveraddr.sin_family = 2;
	serveraddr.sin_addr.s_addr = sceNetHtonl(INADDR_ANY);
	serveraddr.sin_port = sceNetHtons(ps4LinkGetValue(REQUESTS_PORT));
	memset(serveraddr.sin_zero, 0, sizeof(serveraddr.sin_zero));

	/**Enable SCE_NET_SO_REUSEADDR*/
	/*int value=1;
	ret = sceNetSetsockopt(configuration->ps4link_requests_sock, 0xffff, 4, &value, sizeof(value));
	if(ret<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4link_requests_thread sceNetSetsockopt error: 0x%08X\n", ret);
		sceNetSocketClose(ps4LinkGetValue(REQUESTS_SOCK));
		return NULL;
	}*/
	
	/* Bind the server's address to the socket */
	ret = sceNetBind(ps4LinkGetValue(REQUESTS_SOCK), (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(ret<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] sceNetBind error: 0x%08X\n", ret);
		sceNetSocketClose(ps4LinkGetValue(REQUESTS_SOCK));
		return NULL;
	}
	debugNetPrintf(DEBUG,"[PS4LINK] bind to ps4link_requests_sock done\n");
	
	/* Start listening */
	ret = sceNetListen(ps4LinkGetValue(REQUESTS_SOCK), 5);
	if(ret<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] sceNetListen error: 0x%08X\n", ret);
		sceNetSocketClose(ps4LinkGetValue(REQUESTS_SOCK));
		return NULL;
	}
	
	
	debugNetPrintf(DEBUG,"[PS4LINK] Ready for connection %d\n", ps4LinkGetValue(FILEIO_ACTIVE));
   
   
	while(ps4LinkGetValue(FILEIO_ACTIVE))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] Waiting for connection\n", ret);
		
		/* Accept clients */
		struct sockaddr_in clientaddr;
		int client_sock;
		unsigned int addrlen = sizeof(clientaddr);
		
		client_sock = sceNetAccept(ps4LinkGetValue(REQUESTS_SOCK), (struct sockaddr *)&clientaddr, &addrlen);
		if (client_sock < 0) {
			debugNetPrintf(DEBUG,"[PS4LINK] sceNetAccept error (0x%08X)\n", client_sock);
		    continue;
		}
		
		
		/* Get the client's IP address */
		remote_pc_addr = clientaddr.sin_addr.s_addr;
		char remote_ip[16];
		sceNetInetNtop(AF_INET,&clientaddr.sin_addr.s_addr,remote_ip,sizeof(remote_ip));
		debugNetPrintf(DEBUG,"[PS4LINK] Client connected from %s port: %i\n ",remote_ip, clientaddr.sin_port);			
		if (ps4LinkGetValue(FILEIO_SOCK) > 0) {
				debugNetPrintf(DEBUG,"[PS4LINK] Client reconnected\n");
				// sceNetSocketClose(ps4LinkGetValue(FILEIO_SOCK));
		}
		
		configuration->ps4link_fileio_sock = client_sock;
			
		ps4link_requests_connected=1;
		
		debugNetPrintf(DEBUG,"[PS4LINK] sock ps4link_fileio set %d connected %d\n",ps4LinkGetValue(FILEIO_SOCK),ps4link_requests_connected);
		
		
		
	}
	debugNetPrintf(DEBUG,"[PS4LINK] exit thread requests\n");
	if(ps4LinkGetValue(FILEIO_SOCK))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] closing fileio_sock\n");
		sceNetSocketClose(ps4LinkGetValue(FILEIO_SOCK));
		configuration->ps4link_fileio_sock=-1;
	}
	if(ps4LinkGetValue(REQUESTS_SOCK))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] closing server_request_sock\n");
		sceNetSocketClose(ps4LinkGetValue(REQUESTS_SOCK));
		configuration->ps4link_requests_sock=-1;
	}
	
	ps4link_requests_connected=0;
	
	
	//sceKernelExitDeleteThread(0);
	return NULL;
}
