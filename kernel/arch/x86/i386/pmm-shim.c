#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <cedille/pmm.h>
#include <arch/x86/paging.h>
void pmm_shim_alloc_frame(uintptr_t address, int kernel, int rw) {
	//TODO: Translate address to page number
	//TODO: Define interface with actual documentation instead of a source file. Or with a skeleton source file.
}
void pmm_shim_free_frame(uintptr_t address) {
	
}
