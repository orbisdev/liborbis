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
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>


#include <ps4/kernel.h>

#include <kernel.h>
#include <ps4/memory.h>
#include <ps4/error.h>

#include <sys/sysent.h>

//#include <sys/types.h>
#include <sys/sysctl.h>
//#include <sys/user.h>
#include <sys/ptrace.h>


#include <ps4/memory/protected.h>
#include <inttypes.h>
#include <elfloader.h>

#include "ps4link.h"
#include "ps4link_internal.h"
#include "kmain.h"


/*typedef int (*ElfMain)(int argc, char **argv);
typedef void (*ElfProcessMain)(void *arg);

typedef struct ElfRunUserArgument
{
	ElfMain main;
	Ps4MemoryProtected *memory;
	//Ps4Memory *memory;
}
ElfRunUserArgument;

typedef struct ElfRunKernelArgument
{
	int isProcess;
	size_t size;
	void *data;
}
ElfRunKernelArgument;

typedef int (*ElfRunner)(Elf *elf);

typedef struct ElfServerArgument
{
	volatile int *run;
	pthread_t thread;
	ElfRunner elfRunner;
	int descriptor;
	int port;
}
ElfServerArgument;

typedef struct ElfKernelProcessInformation
{
	struct proc *process;
	uint8_t *processMain;
	uint8_t *main;
	int argc;
	char *argv[3];
	char elfName[128];
	void *processFree;
	void *processMemoryType;
	void *processExit;
}
ElfKernelProcessInformation;

typedef void (*ElfProcessExit)(int ret);
typedef void (*ElfProcessFree)(void *m, void *t);
*/
typedef struct {
    int index;
    uint64_t fileoff;
    size_t bufsz;
    size_t filesz;
} SegmentBufInfo;

extern int ftp_initialized;
typedef int (*Runnable)(int, char **);
typedef struct MainAndMemory
{
	Runnable main;
	Ps4MemoryProtected *memory;
}MainAndMemory;
MainAndMemory mm;

ScePthread elf_thid;

int GID=-1;
int UID=-1;

#define BUF_SIZE 1024
static char recvbuf[BUF_SIZE] __attribute__((aligned(16)));
extern ps4LinkConfiguration *configuration;


void *ps4LinkUserMain(void *arg)
{
	ElfRunUserArgument *argument = (ElfRunUserArgument *)arg;
	ps4LinkConfiguration *shared_conf=configuration;
	char pointer_conf[256];
	sprintf(pointer_conf,"%p",shared_conf->debugconf);
	debugNetPrintf(DEBUG,"[PS4LINK] Configuration pointer %p, pointer_conf string %s\n",shared_conf->debugconf,pointer_conf);
	char *elfName = "elf";
	char *elfArgv[3] = { elfName, pointer_conf, NULL };
	int elfArgc = 2;
	
	int r;

	if(argument == NULL)
		return NULL;

	r = argument->main(elfArgc, elfArgv);
	ps4MemoryProtectedDestroy(argument->memory);
	//ps4MemoryDestroy(argument->memory);
	free(argument);
	debugNetPrintf(DEBUG,"return (user): %i\n", r);

	return NULL;
}

int ps4LinkUserRun(Elf *elf)
{
	//pthread_t thread;
	ScePthread thread;
	int ret;
	ElfRunUserArgument *argument;
	void *writable, *executable;
	int r;

	if(elf == NULL)
		return -1;
	debugNetPrintf(DEBUG,"malloc for argument\n");

 	argument = (ElfRunUserArgument *)malloc(sizeof(ElfRunUserArgument));
	if(argument ==  NULL)
	{
		elfDestroyAndFree(elf);
		debugNetPrintf(DEBUG,"argument is NULL\n");
		return -1;
	}
	debugNetPrintf(DEBUG,"after malloc for argument\n");

	if(ps4MemoryProtectedCreate(&argument->memory, elfMemorySize(elf)) != 0)
	//if(ps4MemoryCreate(&argument->memory, elfMemorySize(elf)) != PS4_OK)
	{
		free(argument);
		elfDestroyAndFree(elf);
		debugNetPrintf(DEBUG,"after elfDestroyAndFree\n");
		
		return -1;
	}
	debugNetPrintf(DEBUG,"after ps4MemoryProtectedCreate\n");

	argument->main = NULL;
	ps4MemoryProtectedGetWritableAddress(argument->memory, &writable);
	debugNetPrintf(DEBUG,"after ps4MemoryProtectedGetWritableAddress\n");
	
	ps4MemoryProtectedGetExecutableAddress(argument->memory, &executable);
	debugNetPrintf(DEBUG,"after ps4MemoryProtectedGetExecutableAddress\n");
	
	r = elfLoaderLoad(elf, writable, executable);
	//r = elfLoaderLoad(elf, ps4MemoryGetAddress(argument->memory), ps4MemoryGetAddress(argument->memory));
	debugNetPrintf(DEBUG,"after elfLoaderLoad return r=%d readable=%p executable=%p\n",r,writable,executable);
	
	if(r == ELF_LOADER_RETURN_OK)
	{
		argument->main = (ElfMain)((uint8_t *)executable + elfEntry(elf));
		debugNetPrintf(DEBUG,"after set argument->main %p \n",argument->main);
		
		//argument->main = (ElfMain)((uint8_t *)ps4MemoryGetAddress(argument->memory) + elfEntry(elf));
	}
	//elfDestroyAndFree(elf); // we don't need the "file" anymore but if i leave this line i got a memory crash
	debugNetPrintf(DEBUG,"after elfDestroyAndFree \n");

	if(argument->main != NULL)
	{	//pthread_create(&thread, NULL, elfLoaderUserMain, argument);
		ret=scePthreadCreate(&thread, NULL, ps4LinkUserMain, argument, "elf_user_thid");
		if(ret==0)
		{
			debugNetPrintf(DEBUG,"[PS4LINK] New user elf thread UID: 0x%08X\n", thread);			
		}
		else
		{
			debugNetPrintf(DEBUG,"[PS4LINK] New user elf thread could not create error: 0x%08X\n", ret);
			scePthreadCancel(thread);
			//ps4LinkFinish();
			return PS4_NOT_OK;
		}
	}
	else
	{
		ps4MemoryProtectedDestroy(argument->memory);
		//ps4MemoryDestroy(argument->memory);
		free(argument);
		debugNetPrintf(DEBUG,"argument->main is NULL\n");
		
		return -1;
	}
	return PS4_OK;
}


/* Kernel & process elf runner */ //FIXME: checks

void *ps4LinkKernelMain(void *arg)
{
	int p;
	int64_t r, ret;
	ElfRunKernelArgument *ka = (ElfRunKernelArgument *)arg;
	ps4KernelCall(ps4KernelMemoryCopy, &ka->isProcess, &p, sizeof(int)); // ka is in kernel
	ret = 0;
	r = ps4KernelExecute((void *)ps4LinkKernMain, ka, &ret, NULL);
	if(p == 0)
		debugNetPrintf(DEBUG,"return (kernel): %i %"PRId64"\n", r, ret);
	else
		debugNetPrintf(DEBUG,"return (kernel process): %i %"PRId64"\n", r, ret); //FIXME: r should be 0 ...?!
	return NULL;
}

int ps4LinkKernelRun_(Elf *elf, int asProcess)
{
	int ret;
	//pthread_t thread;
	ScePthread thread;
	
	ElfRunKernelArgument ua;
	ElfRunKernelArgument *ka;

	if(elf == NULL)
		return -1;

	ka = (void *)ps4KernelCall(ps4KernelMemoryMalloc, sizeof(ElfRunKernelArgument));
	ua.isProcess = asProcess;
	ua.size = elfGetSize(elf);
	ua.data = (void *)ps4KernelCall(ps4KernelMemoryMalloc, ua.size);
	ps4KernelCall(ps4KernelMemoryCopy, elfGetData(elf), ua.data, ua.size);
	ps4KernelCall(ps4KernelMemoryCopy, &ua, ka, sizeof(ElfRunKernelArgument));

	elfDestroyAndFree(elf); // we dispose of non-kernel data and rebuild and clean the elf in kernel

	//pthread_create(&thread, NULL, ps4LinkKernelMain, ka);
	ret=scePthreadCreate(&thread, NULL, ps4LinkKernelMain, ka, "elf_thid");
	if(ret==0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] New elf thread UID: 0x%08X\n", thread);			
	}
	else
	{
		debugNetPrintf(DEBUG,"[PS4LINK] New elf thread could not create error: 0x%08X\n", ret);
		scePthreadCancel(thread);
		//ps4LinkFinish();
		return PS4_NOT_OK;
	}
	return PS4_OK;
}
int ps4LinkKernelRun(Elf *elf)
{
	return ps4LinkKernelRun_(elf, 0);
}

int ps4LinkKernelProcessRun(Elf *elf)
{
	return ps4LinkKernelRun_(elf, 1);
}

Elf * ps4LinkReadElfFromHost(char *path)
{
	int fd; //descriptor to manage file from host0
	int filesize;//variable to control file size 
	uint8_t *buf=NULL;//buffer for read from host0 file
	Elf *elf;//elf to create from buf 
	
	//we open file in read only from host0 ps4sh include the full path with host0:/.......
	fd=ps4LinkOpen(path,O_RDONLY,0);

	//If we can't open file from host0 print  the error and return
	if(fd<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkOpen returned error %d\n",fd);
		return NULL;
	}
	//Seek to final to get file size
	filesize=ps4LinkLseek(fd,0,SEEK_END);
	//If we get an error print it and return
	if(filesize<0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkSeek returned error %d\n",fd);
		ps4LinkClose(fd);
		return NULL;
	}
	//Seek back to start
	ps4LinkLseek(fd,0,SEEK_SET);
	//Reserve  memory for read buffer
	//buf=malloc(filesize);
	//char buf[filesize];
	buf=mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	
	
	//Read filsesize bytes to buf
	int numread=ps4LinkRead(fd,buf,filesize);
	//if we don't get filesize bytes we are in trouble
	if(numread!=filesize)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkRead returned error %d\n",numread);
		ps4LinkClose(fd);
		return NULL;		
	}
	//Close file
	ps4LinkClose(fd);
	//create elf from elfloader code from hitodama :P
	elf = elfCreate((void*)buf, filesize);
	//check is it is loadable
	if(!elfLoaderIsLoadable(elf))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] elf %s is not loadable\n",path);
		//free(buf);
		elfDestroy(elf);
		elf = NULL;
		
	}
	
	
	return elf;

	
}
void ps4LinkCmdExecKernelElf(ps4link_pkt_exec_cmd *pkg)
{

	Elf *elf=NULL;
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execkernelelf argc=%x argv=%s\n",ntohl(pkg->argc),pkg->argv);
	elf=ps4LinkReadElfFromHost(pkg->argv);
	if(elf==NULL)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] we can't create elf\n");
	}
	debugNetPrintf(DEBUG,"[PS4LINK] ready to run elf\n");
	ps4LinkKernelRun(elf);
	return;
}
void ps4LinkCmdExecUserElf(ps4link_pkt_exec_cmd *pkg)
{

	Elf *elf=NULL;
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execuserelf argc=%x argv=%s\n",ntohl(pkg->argc),pkg->argv);
	elf=ps4LinkReadElfFromHost(pkg->argv);
	if(elf==NULL)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] we can't create elf\n");
	}
	debugNetPrintf(DEBUG,"[PS4LINK] ready to run elf\n");
	ps4LinkUserRun(elf);
	return;
}
char *entryName(int entryType) {
	switch(entryType) {
		case DT_DIR: return "DIR";
		case DT_REG: return "FILE";
		default: return "OTHER";
	}
}
void ps4LinkCmdExecWhoami(ps4link_pkt_exec_cmd *pkg)
{
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execwhoami\n");
	UID=getuid();
	GID=getgid();
	
	debugNetPrintf(DEBUG,"[PS4LINK] UID: %d, GID: %d\n",UID,GID);
		
}
void ps4LinkCmdExecShowDir(ps4link_pkt_exec_cmd *pkg)
{
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execshowdir\n");
	char *buffer;
	struct dirent *dent;
	struct stat stats;
	int dfd;
	int i;
	UID=getuid();
	GID=getgid();
	if(pkg->argv[0]!='\0')
	{
		
		dfd = open(pkg->argv, O_RDONLY, 0);
		if(dfd < 0) {
			debugNetPrintf(DEBUG, "Invalid directory.\n");
			return;
		}
		int err=fstat(dfd, &stats);
		if(err<0)
		{
			debugNetPrintf(DEBUG, "fstat error return  0x%08X \n",err);
			return;
		}
		buffer=mmap(NULL, stats.st_blksize+sizeof(struct dirent), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (buffer)
		{
			// Make sure we will have a null terminated entry at the end.Thanks people leaving CryEngine code for orbis on github  :)
			for(i=0;i<stats.st_blksize+sizeof(struct dirent);i++)
			{
				buffer[i]=0;
			}
			err=getdents(dfd, buffer, stats.st_blksize);

			int nOffset = err;
			while (err > 0 && err < stats.st_blksize)
			{
				err = getdents(dfd, buffer + nOffset, stats.st_blksize-nOffset);
				nOffset += err;
			}
			
			if (err>0)
				err=0;
			
			dent = (struct dirent *)buffer;
			while(dent->d_fileno ) {
					debugNetPrintf(DEBUG, "[%s]: %s\n", entryName(dent->d_type), dent->d_name);

					dent = (struct dirent *)((void *)dent + dent->d_reclen);
			}
		}
		munmap(buffer,stats.st_blksize+sizeof(struct dirent));
		
		
		
		

		debugNetPrintf(DEBUG,"[PS4LINK] closing dfd\n");

		err=close(dfd);  
		if(err<0)
		{
				debugNetPrintf(DEBUG, "fstat error return  0x%08X \n",err);
					return;
		}
	}
	else
	{
		if(pkg->argv[0]!='\0')
		{
			debugNetPrintf(DEBUG,"Sorry you are not root , you must be root to run this...\n");
		}
		else
		{
			debugNetPrintf(DEBUG,"Sorry you must provide a ps4 directory path...\n");
			
		}
	}
	debugNetPrintf(DEBUG,"[PS4LINK] end command execshowdir\n");
	
}


void print_phdr(Elf64_Phdr *phdr) {
    printf("=================================\n");
    printf("     p_type %08x\n", phdr->p_type);
    printf("     p_flags %08x\n", phdr->p_flags);
    printf("     p_offset %016llx\n", phdr->p_offset);
    printf("     p_vaddr %016llx\n", phdr->p_vaddr);
    printf("     p_paddr %016llx\n", phdr->p_paddr);
    printf("     p_filesz %016llx\n", phdr->p_filesz);
    printf("     p_memsz %016llx\n", phdr->p_memsz);
    printf("     p_align %016llx\n", phdr->p_align);
}


int read_decrypt_segment(int fd, uint64_t index, uint64_t offset, size_t size, uint8_t *out) 
{
	uint64_t realOffset = (index << 32) | offset;
	uint8_t *addr = (uint8_t*)mmap(0, size, PROT_READ, MAP_PRIVATE | 0x80000, fd, realOffset);
	if (addr != MAP_FAILED) {
		memcpy(out, addr, size);
		munmap(addr, size);
		return TRUE;
	}
	else 
	{
		debugNetPrintf(DEBUG,"[PS4LINK] mmap segment [%d] err(%d) : %s\n", index, errno, strerror(errno));
		return FALSE;
	}
}



int is_segment_in_other_segment(Elf64_Phdr *phdr, int index, Elf64_Phdr *phdrs, int num) 
{
	for (int i = 0; i < num; i += 1) 
	{
		Elf64_Phdr *p = &phdrs[i];
		if (i != index) 
		{
			if (p->p_filesz > 0) 
			{
				// printf("offset : %016x,  toffset : %016x\n", phdr->p_offset, p->p_offset);
				// printf("offset : %016x,  toffset + size : %016x\n", phdr->p_offset, p->p_offset + p->p_filesz);
				if ((phdr->p_offset >= p->p_offset) && ((phdr->p_offset + phdr->p_filesz) <= (p->p_offset + p->p_filesz))) 
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


SegmentBufInfo *parse_phdr(Elf64_Phdr *phdrs, int num, int *segBufNum) 
{
	
	debugNetPrintf(DEBUG,"[PS4LINK] segment num : %d\n", num);
	SegmentBufInfo *infos = (SegmentBufInfo *)malloc(sizeof(SegmentBufInfo) * num);
	int segindex = 0;
	for (int i = 0; i < num; i += 1) 
	{
		Elf64_Phdr *phdr = &phdrs[i];
		// print_phdr(phdr);

		if (phdr->p_filesz > 0 && phdr->p_type != 0x6fffff01) 
		{
			if (!is_segment_in_other_segment(phdr, i, phdrs, num)) 
			{
				SegmentBufInfo *info = &infos[segindex];
				segindex += 1;
				info->index = i;
				info->bufsz = (phdr->p_filesz + (phdr->p_align - 1)) & (~(phdr->p_align - 1));
				info->filesz = phdr->p_filesz;
				info->fileoff = phdr->p_offset;

				// printf("seg buf info %d -->\n", segindex);
				// printf("    index : %d\n    bufsz : 0x%016llX\n", info->index, info->bufsz);
				// printf("    filesz : 0x%016llX\n    fileoff : 0x%016llX\n", info->filesz, info->fileoff);
			}
		}
	}
	*segBufNum = segindex;
	return infos;
}


void do_dump(char *saveFile, int fd, SegmentBufInfo *segBufs, int segBufNum, Elf64_Ehdr *ehdr) 
{
	int sfd;
	sfd=ps4LinkOpen(saveFile, O_RDWR|O_CREAT, 0);
	if (sfd>0) 
	{
		size_t elfsz = 0x40 + ehdr->e_phnum * sizeof(Elf64_Phdr);
		debugNetPrintf(DEBUG,"[PS4LINK] elf header + phdr size : 0x%08X\n", elfsz);
		ps4LinkWrite(sfd,ehdr, elfsz);

		for (int i = 0; i < segBufNum; i += 1) {
			debugNetPrintf(DEBUG,"[PS4LINK] sbuf index : %d, offset : 0x%016x, bufsz : 0x%016x, filesz : 0x%016x\n", segBufs[i].index, segBufs[i].fileoff, segBufs[i].bufsz, segBufs[i].filesz);
			uint8_t *buf = (uint8_t*)malloc(segBufs[i].bufsz);
			memset(buf, 0, segBufs[i].bufsz);
			if (read_decrypt_segment(fd, segBufs[i].index, 0, segBufs[i].filesz, buf)) {
				ps4LinkLseek(sfd,segBufs[i].fileoff,SEEK_SET);
				ps4LinkWrite(sfd,buf,segBufs[i].bufsz);
			}
			free(buf);
		}
		ps4LinkClose(sfd);
	}
    else 
	{
		debugNetPrintf(DEBUG,"[PS4LINK] ps4LinkOpen error opening %s\n", saveFile);
    }
}
void ps4LinkCmdExecDecrypt(ps4link_pkt_exec_cmd *pkg)
{
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execdecrypt argc=%x argv=%s\n",ntohl(pkg->argc),pkg->argv);
	char *filename;
	char savefile[256];
	int fd;
    int64_t ret;
	if(UID==0 && GID==0 && pkg->argv[0]!='\0')
	{
		filename=(strrchr(pkg->argv, '/'));
		if(!filename)
		{
			debugNetPrintf(DEBUG,"[PS4LINK] basename %s is invalid must begin with / \n",pkg->argv);
			return;
		}
		filename=filename+1;
		debugNetPrintf(DEBUG,"[PS4LINK] file name to decrypt %s\n",filename);
		sprintf(savefile,"host0:%s",filename);
		debugNetPrintf(DEBUG,"[PS4LINK] savefile in your host %s\n",savefile);
		
		debugNetPrintf(DEBUG,"[PS4LINK] kernel hook\n");
	    ps4KernelExecute((void*)path_self_mmap_check_function, NULL, &ret, NULL);

		
		fd=open(pkg->argv, O_RDONLY);
		if(fd!=-1)
		{
			void *addr = mmap(0, 0x4000, PROT_READ, MAP_PRIVATE, fd, 0);
			if (addr != MAP_FAILED) 
			{
				debugNetPrintf(DEBUG,"[PS4LINK] mmap %s : %p\n", pkg->argv, addr);
				uint16_t snum = *(uint16_t*)((uint8_t*)addr + 0x18);
				Elf64_Ehdr *ehdr = (Elf64_Ehdr *)((uint8_t*)addr + 0x20 + snum * 0x20);
				debugNetPrintf(DEBUG,"[PS4LINK] ehdr : %p\n", ehdr);
				
				Elf64_Phdr *phdrs = (Elf64_Phdr *)((uint8_t *)ehdr + 0x40);
				debugNetPrintf(DEBUG,"[PS4LINK] phdrs : %p\n", phdrs);
				
				int segBufNum = 0;
				SegmentBufInfo *segBufs = parse_phdr(phdrs, ehdr->e_phnum, &segBufNum);
				do_dump(savefile, fd, segBufs, segBufNum, ehdr);
				debugNetPrintf(DEBUG,"[PS4LINK] dump completed\n");
				
				free(segBufs);
				munmap(addr, 0x4000);
				close(fd);
	        }
			else 
			{
				debugNetPrintf(DEBUG,"mmap file %s err : %s\n", pkg->argv, strerror(errno));
			}
		}
		else
		{
			debugNetPrintf(DEBUG,"[PS4LINK] open %s err : %s\n", pkg->argv, strerror(errno));
			
		}
	    
		debugNetPrintf(DEBUG,"[PS4LINK] kernel unhook\n");
	    ps4KernelExecute((void*)unpath_self_mmap_check_function, NULL, &ret, NULL);
		
	}
	debugNetPrintf(DEBUG,"[PS4LINK] end command execdecrypt\n");
	
}
void ps4LinkCmdExecPtrace(ps4link_pkt_exec_cmd *pkg)
{
	debugNetPrintf(DEBUG,"[PS4LINK] Received command execptrace argc=%x argv=%s\n",ntohl(pkg->argc),pkg->argv);
	int i;
	int ret;
	int mib[4];
	size_t len=0;
	uint64_t start_address=0;
	uint64_t end_address=0;
	uint64_t elf_size=0; 
	struct ptrace_io_desc pt_desc;
	void* dump;
	
	for(i = 55; i < 200; i++) 
	{
		
		mib[0]=CTL_KERN;
		mib[1]=KERN_PROC;
		mib[2]=KERN_PROC_PID;
		mib[3]=i;
	
		if(sysctl(mib,4,NULL,&len,NULL,0)!=-1) 
		{
			if(len>0) 
			{
				dump= malloc(len);
				if(sysctl(mib,4,dump,&len,NULL,0)!=-1) 
				{
					char* name=dump+0x1bf;
					if(strcmp(name,"eboot.bin")==0) 
					{
						debugNetPrintf(INFO,"[PS4LINK] found eboot.bin pid %d\n",i);
						free(dump);
						break;
					}
				}
				free(dump);
			}
		}
	}
	len=0;
	mib[0]=CTL_KERN;
	mib[1]=KERN_PROC;
	mib[2]=KERN_PROC_VMMAP;
	mib[3]=i;
	if(sysctl(mib, 4, NULL, &len, NULL, 0)!= -1)
	{
		if(len>0)
		{
			dump=malloc(len);
			if(!dump)
			{
				debugNetPrintf(ERROR,"[PS4LINK] malloc error\n");
				return;
			}
			if(sysctl(mib,4,dump,&len,NULL,0)==-1) 
			{
				debugNetPrintf(ERROR,"[PS4LINK] sysctl vm return -1\n");
				free(dump);
				return;
			}
			start_address=*(uint64_t *)(dump+8);
			end_address=*(uint64_t *)(dump+144+144+16);
			elf_size=end_address-start_address; 
			free(dump);
		}	
	}
	else
	{
		debugNetPrintf(ERROR,"[PS4LINK] sysctl vm return -1\n");
		return;
	}
	debugNetPrintf(INFO,"[PS4LINK] attaching to eboot.bin pid %d\n",i);
	
	ret=ptrace(PT_ATTACH,i,NULL,NULL);
	
	debugNetPrintf(INFO,"[PS4LINK] ptrace return 0x%08X\n",ret);
	
	debugNetPrintf(INFO,"[PS4LINK] read eboot.bin from %x to %x with size %x\n",start_address,end_address,elf_size);
	dump=malloc(elf_size);
	if(dump)
	{
		pt_desc.piod_op=PIOD_READ_D;
		pt_desc.piod_addr=dump;
		pt_desc.piod_offs=start_address;
		pt_desc.piod_len=elf_size;
		ret=ptrace(PT_IO,i,&pt_desc,NULL);
		debugNetPrintf(INFO,"[PS4LINK] ptrace return 0x%08X\n",ret);
		debugNetPrintf(INFO,"[PS4LINK] ptrace detach\n");
		int sfd;
		sfd=ps4LinkOpen("host0:eboot.bin.elf", O_RDWR|O_CREAT, 0);
		if(sfd>0)
		{
			ps4LinkWrite(sfd,dump,elf_size);
			ps4LinkClose(sfd);
		}
		ret=ptrace(PT_DETACH,i,NULL,NULL);
		debugNetPrintf(INFO,"[PS4LINK] ptrace return 0x%08X\n",ret);
		
	}
}
void ps4LinkCmdExit(ps4link_pkt_exec_cmd *pkg)
{
	debugNetPrintf(DEBUG,"[PS4LINK] Received command exit. Closing PS4Link...\n");
	
	ps4LinkFinish();
	
}
void *ps4link_commands_thread(void* args)
{
	struct sockaddr_in serveraddr;
	//    struct sockaddr_in remote_addr;
	int ret;
	int len;
	unsigned int addrlen;
	unsigned int cmd;
	ps4link_pkt_hdr *header;
	struct sockaddr_in remote_addr;
	
	
	
	
	debugNetPrintf(DEBUG,"[PS4LINK] Command Thread Started.\n" );
	
	configuration->ps4link_commands_sock = sceNetSocket("commands_server_sock",AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if (ps4LinkGetValue(COMMANDS_SOCK) >=0)
	{
		debugNetPrintf(DEBUG,"[PS4LINK] Created ps4link_commands_sock: %d\n", ps4LinkGetValue(COMMANDS_SOCK));
	}
	else
	{
		debugNetPrintf(DEBUG,"[PS4LINK] Error creating socket ps4link_commands_sock  %d\n", ps4LinkGetValue(COMMANDS_SOCK));
		ps4LinkFinish();
		return NULL;
	}
	/* Fill the server's address */
    memset(&serveraddr, 0, sizeof serveraddr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = sceNetHtonl(INADDR_ANY);
	serveraddr.sin_port = sceNetHtons(ps4LinkGetValue(COMMANDS_PORT));
	memset(&(serveraddr.sin_zero), '\0', 8);
	
	
   
	ret = sceNetBind(ps4LinkGetValue(COMMANDS_SOCK), (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	
	
	
	if (ret < 0) 
	{
		debugNetPrintf(DEBUG,"[PS4LINK] command listener sceNetBind error: 0x%08X\n", ret);
		sceNetSocketClose(ps4LinkGetValue(COMMANDS_SOCK));
		ps4LinkFinish();
		return NULL;
	}
	// Do tha thing
	debugNetPrintf(DEBUG,"[PS4LINK] Command listener waiting for commands...\n");
	
	
	
	
	
	
	


	while(ps4LinkGetValue(CMDSIO_ACTIVE)) {

		addrlen = sizeof(remote_addr);
		//wait for new command
		
		len = recvfrom(ps4LinkGetValue(COMMANDS_SOCK), &recvbuf[0], BUF_SIZE, 0, (struct sockaddr *)&remote_addr,&addrlen);
		
		debugNetPrintf(DEBUG,"[PS4LINK] commands listener received packet size (%d)\n", len);

		if (len < 0) {
			debugNetPrintf(DEBUG,"[PS4LINK] commands listener recvfrom size error (%d)\n", len);
			continue;
		}
		if (len < sizeof(ps4link_pkt_hdr)) {
			debugNetPrintf(DEBUG,"[PS4LINK] commands listener recvfrom header size error (%d)\n", len);
			continue;
		}

		header = (ps4link_pkt_hdr *)recvbuf;
		cmd = ntohl(header->cmd);
		
		switch (cmd) {

			case PS4LINK_EXECUSER_CMD:
				ps4LinkCmdExecUserElf((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXECKERNEL_CMD:
				ps4LinkCmdExecKernelElf((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXIT_CMD:
				ps4LinkCmdExit((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXECWHOAMI_CMD:
				ps4LinkCmdExecWhoami((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXECSHOWDIR_CMD:
				ps4LinkCmdExecShowDir((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXECDECRYPT_CMD:
				ps4LinkCmdExecDecrypt((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			case PS4LINK_EXECPTRACE_CMD:
				ps4LinkCmdExecPtrace((ps4link_pkt_exec_cmd *)recvbuf);
				break;
			default: 
				debugNetPrintf(DEBUG,"[PS4LINK] Unknown command received\n");
				break;
		}
		debugNetPrintf(DEBUG,"[PS4LINK] commands listener waiting for next command\n");
	}
	debugNetPrintf(DEBUG,"[PS4LINK] exit commands listener thread\n");
	if(ps4LinkGetValue(COMMANDS_SOCK))
	{
		debugNetPrintf(DEBUG,"[PS4LINK] closing server_commands_sock\n");
		sceNetSocketClose(ps4LinkGetValue(COMMANDS_SOCK));
		configuration->ps4link_commands_sock=-1;
	}
	
	//sceKernelExitDeleteThread(0);
	return NULL;
}
