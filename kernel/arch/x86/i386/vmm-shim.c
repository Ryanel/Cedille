#include <cedille/pmm.h>
#include <arch/x86/paging.h>
int vmm_shim_doBoardSetup() {
	int ret = paging_setup();
	return ret;
}