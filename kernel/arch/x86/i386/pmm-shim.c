#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <cedille/pmm.h>
#include <cedille/logging.h>
#include <arch/x86/paging.h>
extern page_directory_t * current_directory;

void pmm_ShimAllocFrame(uintptr_t address, int kernel, int rw) {
	//TODO: Translate address to page number
	page_t * page = paging_get_page(address, 1, current_directory);
	uint32_t index = INDEX_FROM_BIT(address);
	page->present = 1;
	page->rw = (rw)?1:0; // Should the page be writeable?
	page->user = (kernel)?0:1; // Should the page be user-mode?
	page->frame = index;
	
}
void pmm_ShimFreeFrame(uintptr_t address) {
	printk(LOG_WARN,"pmm_shim","Attempted to free frame 0x%X\n",address);
}
