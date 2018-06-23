#undef _SYS_CDEFS_H_
#undef _SYS_TYPES_H_
#undef _SYS_PARAM_H_
#undef _SYS_MALLOC_H_

#define _XOPEN_SOURCE 700
#define __BSD_VISIBLE 1
#define _KERNEL
#define _WANT_UCRED

#include <sys/cdefs.h>
#include <sys/types.h>
// #include <sys/param.h>
// #include <sys/kernel.h>
// #include <sys/systm.h>
// #include <sys/lock.h>
// #include <sys/mutex.h>
// #include <sys/proc.h>
// #include <sys/malloc.h>
// #include <sys/uio.h>
// #include <sys/filedesc.h>
// #include <sys/file.h>

#include <ps4/kernel.h>
#include <elfloader.h>
#include "kmain.h"

int myhook1(struct thread *td, Ps4KernelFunctionHookArgument *arg) 
{
	arg->returns->rax = 1;
	return PS4_KERNEL_FUNCTION_HOOK_CONTROL_CONTINUE;
}

int myhook2(struct thread *td, Ps4KernelFunctionHookArgument *arg) 
{
	arg->returns->rax = 0;
	return PS4_KERNEL_FUNCTION_HOOK_CONTROL_CONTINUE;
}

int path_self_mmap_check_function(struct thread *td, void *uap) 
{
	void *func1 = ps4KernelDlSym("sceSblACMgrIsAllowedToMmapSelf");
	void *func2 = ps4KernelDlSym("sceSblAuthMgrIsLoadable");
	ps4KernelFunctionPosthook(func1, myhook1);
	ps4KernelFunctionPosthook(func2, myhook2);
	return 0;
}

int unpath_self_mmap_check_function(struct thread *td, void *uap) 
{
	void *func1 = ps4KernelDlSym("sceSblACMgrIsAllowedToMmapSelf");
	void *func2 = ps4KernelDlSym("sceSblAuthMgrIsLoadable");
	ps4KernelFunctionUnhook(func1);
	ps4KernelFunctionUnhook(func2);
	return 0;
}

void elfPayloadProcessMain(void *arg)
{
	ElfKernelProcessInformation *pargs = arg;
	//ElfProcessExit pexit = (ElfProcessExit)pargs->processExit;
	((ElfMain)pargs->main)(pargs->argc, pargs->argv);
	((ElfProcessFree)pargs->processFree)(pargs, pargs->processMemoryType);
	//pexit(0); //FIXME: Hmm? Oo -> panics, should not, example sys/dev/mmc/mmcsd.c
}
int ps4LinkKernMain(struct thread *td, void *uap)
{
	ElfRunKernelArgument *arg;
	ElfKernelProcessInformation *pargs;
	char buf[32];
	Elf *elf;
	int isProcess, r;
	int elfPayloadProcessMainSize = 64; // adapt if needed

	arg = (ElfRunKernelArgument *)uap;
	if(arg == NULL || arg->data == NULL)
		return PS4_ERROR_ARGUMENT_MISSING;

	isProcess = arg->isProcess;
 	elf = elfCreateLocalUnchecked((void *)buf, arg->data, arg->size);

	pargs = ps4KernelMemoryMalloc(sizeof(ElfKernelProcessInformation) + elfPayloadProcessMainSize + elfMemorySize(elf));
	if(pargs == NULL)
	{
		ps4KernelMemoryFree(arg->data);
		ps4KernelMemoryFree(arg);
		return PS4_ERROR_KERNEL_OUT_OF_MEMORY;
	}

	// memory = (info, procmain, main)
	pargs->processMain = (uint8_t *)pargs + sizeof(ElfKernelProcessInformation);
	pargs->main = pargs->processMain + elfPayloadProcessMainSize;
	r = elfLoaderLoad(elf, pargs->main, pargs->main); // delay error return til cleanup
	pargs->main += elfEntry(elf);


	/*ps4LinkConfiguration *shared_conf=configuration;
	char pointer_conf[256];
	sprintf(pointer_conf,"%p",shared_conf->debugconf);
	// aux
	pargs->argc = 2;
	pargs->argv[0] = pargs->elfName;
	pargs->argv[1] = pointer_conf;
	pargs->argv[2] = NULL;
	sprintf(pargs->elfName, "ps4sdk-elf-%p", pargs);*/

	// Free user argument
	ps4KernelMemoryFree(arg->data);
	ps4KernelMemoryFree(arg);

	if(r != ELF_LOADER_RETURN_OK)
	{
		ps4KernelMemoryFree(pargs);
		return r;
	}

	if(!isProcess)
	{
		int r;
		r = ((ElfMain)pargs->main)(pargs->argc, pargs->argv);
		ps4KernelMemoryFree(pargs);
		ps4KernelThreadSetReturn(td, (register_t)r);
		return PS4_OK;
	}

	ps4KernelSymbolLookUp("free", &pargs->processFree);
	ps4KernelSymbolLookUp("M_TEMP", &pargs->processMemoryType);
	ps4KernelSymbolLookUp("kproc_exit", &pargs->processExit);
	ps4KernelMemoryCopy((void *)elfPayloadProcessMain, pargs->processMain, elfPayloadProcessMainSize);

	if(kproc_create((ElfProcessMain)pargs->processMain, pargs, &pargs->process, 0, 0, "ps4sdk-elf-%p", pargs) != 0)
	{
		ps4KernelMemoryFree(pargs);
		return PS4_ERROR_KERNEL_PROCESS_NOT_CREATED;
	}

	ps4KernelThreadSetReturn(td, (register_t)pargs->process); // FIXME: Races against free
	return PS4_OK; //FIXME: This does not return 0 Oo?
}



