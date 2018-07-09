#include <stdint.h>
#include <kernel.h>
#include <sys/mman.h>
#include <debugnet.h>

#define	CTL_KERN	1	/* "high kernel": proc, limits */
#define	KERN_PROC	14	/* struct: process entries */
#define	KERN_PROC_VMMAP	32	/* VM map entries for process */
#define	KERN_PROC_PID	1	/* by process id */
#define kprintf(format, ...) \
{ \
	int size = sprintf(kprintfbuffer, format, ##__VA_ARGS__); \
	int (*sys_sendto)(struct thread *td, struct sendto_args *uap) = (void *)0xFFFFFFFF8249EC10; \
	struct sendto_args args = { \
		misock, \
		kprintfbuffer, \
		size, \
		0, \
		NULL, \
		0 \
	}; \
	sys_sendto(td, &args); \
}

struct auditinfo_addr {
    /*
    4    ai_auid;
    8    ai_mask;
    24    ai_termid;
    4    ai_asid;
    8    ai_flags;r
    */
    char useless[184];
};



unsigned int long long __readmsr(unsigned long __register) {
	// Loads the contents of a 64-bit model specific register (MSR) specified in
	// the ECX register into registers EDX:EAX. The EDX register is loaded with
	// the high-order 32 bits of the MSR and the EAX register is loaded with the
	// low-order 32 bits. If less than 64 bits are implemented in the MSR being
	// read, the values returned to EDX:EAX in unimplemented bit locations are
	// undefined.
	unsigned long __edx;
	unsigned long __eax;
	__asm__ ("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}


struct ucred {
	uint32_t useless1;
	uint32_t cr_uid;     // effective user id
	uint32_t cr_ruid;    // real user id
 	uint32_t useless2;
    	uint32_t useless3;
    	uint32_t cr_rgid;    // real group id
    	uint32_t useless4;
    	void *useless5;
    	void *useless6;
    	void *cr_prison;     // jail(2)
    	void *useless7;
    	uint32_t useless8;
    	void *useless9[2];
    	void *useless10;
    	struct auditinfo_addr useless11;
    	uint32_t *cr_groups; // groups
    	uint32_t useless12;
};

struct filedesc {
	void *useless1[3];
    	void *fd_rdir;
    	void *fd_jdir;
};

struct proc {
    	char useless[64];
    	struct ucred *p_ucred;
    	struct filedesc *p_fd;
};

struct thread {
    	void *useless;
    	struct proc *td_proc;
};

struct kdump_args{
        uint64_t argArrayPtr;
};

int jailbreak(struct thread *td, struct kpayload_args* args)
{

    struct ucred* cred;
    struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	uint8_t* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-0x30EB30];
	void** got_prison0 =   (void**)&kernel_base[0xF26010];
	void** got_rootvnode = (void**)&kernel_base[0x206D250];

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_prison = *got_prison0;
	fd->fd_rdir = fd->fd_jdir = *got_rootvnode;
    return 0;
}

void myJailbreak()
{
	int ret;
	struct thread *td=NULL;
	debugNetPrintf(3,"td %x",td);
	
	
	 uint64_t* dump = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	debugNetPrintf(3,"dump %x",dump);
	debugNetPrintf(3,"td %x",td);
	
	ret=syscall(11,jailbreak,td,dump);
	debugNetPrintf(3,"ret %d",ret);
	
}

