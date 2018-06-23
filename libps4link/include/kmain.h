#ifndef KMainH
#define KMainH
#include <ps4/memory.h>
#include <ps4/error.h>

#include <sys/sysent.h>
/* Types */

typedef int (*ElfMain)(int argc, char **argv);
typedef void (*ElfProcessMain)(void *arg);
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

int path_self_mmap_check_function(struct thread *td, void *uap);
int unpath_self_mmap_check_function(struct thread *td, void *uap);
int ps4LinkKernMain(struct thread *td, void *uap);
#endif
