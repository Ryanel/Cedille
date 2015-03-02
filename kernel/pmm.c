#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <cedille/pmm.h>
#include <cedille/kconfig.h>
#include <cedille/heap.h>
#include <cedille/logging.h>
#ifdef ARCHx86
#include <arch/x86/paging.h>
#endif
uintptr_t *bitmap 			= NULL; //Pointer to first frame, first index. The actual bitmap
uintptr_t frame_amount 		= 0; //How many frames CAN there be?
uintptr_t mem_end 			= CONFIG_PMM_DEFAULT_MAXMEM; //Where does memory end. Default's to all addressable ram
uintptr_t mem_end_aligned 	= CONFIG_PMM_DEFAULT_MAXMEM; //Where does memory end, page aligned.

uintptr_t pmm_pfaGetTotalPages() {
	return frame_amount;
}

void pmm_pfaBitmapSetFrame(uintptr_t address) {
	uintptr_t frame_addr = address / 0x1000;
	uintptr_t index = INDEX_FROM_BIT(frame_addr);
	uintptr_t offset = OFFSET_FROM_BIT(frame_addr);
	bitmap[index] |= (0x1 << offset);
}
void pmm_pfaBitmapClearFrame(uintptr_t address) {
	uintptr_t frame_addr = address / 0x1000;
	uintptr_t index = INDEX_FROM_BIT(frame_addr);
	uintptr_t offset = OFFSET_FROM_BIT(frame_addr);
	bitmap[index] &= ~(0x1 << offset);
}
uintptr_t pmm_pfaBitmapTestFrame(uintptr_t address) {
	uintptr_t frame_addr = address / 0x1000;
	uintptr_t index = INDEX_FROM_BIT(frame_addr);
	uintptr_t offset = OFFSET_FROM_BIT(frame_addr);
	return (bitmap[index] & (0x1 << offset));
}

uintptr_t pmm_pfaBitmapGetFirstFrame() {
	uintptr_t i,j;
	for (i = 0; i < INDEX_FROM_BIT(frame_amount); i++) {
		if(bitmap[i] != 0xFFFFFFFF) {
			for(j = 0; j < 32; j++) {
				uintptr_t testFrame = 0x1 << j;
				if (!(bitmap[i] & testFrame)) {
					return i * 0x20 + j;
				}
			}
		}
	}
	return -1;
}

void pmm_pfaAllocateSinglePage(uintptr_t address) {
	pmm_pfaBitmapSetFrame(address);
	pmm_ShimAllocFrame(address, 1, 1);
}

uintptr_t * pmm_pfaAllocatePages(unsigned int n) {
	if(n < 2) {
		return 0;
	}
	uintptr_t potential_first = pmm_pfaBitmapGetFirstFrame() * 0x1000;
	for(unsigned int i = 0; i != n; i++) {
		if(pmm_pfaBitmapTestFrame(potential_first + (0x1000 * i)) == 1) {
			potential_first = (potential_first + (0x1000 * i));
		}
	}
	// It passes, keep potential first and return it after setting used bits.
	for(unsigned int i = 0; i != n; i++) {
		pmm_pfaAllocateSinglePage(potential_first + (0x1000 * i));
	}
	printf("Potential first:0x%X\n",potential_first);
	return (uintptr_t*)potential_first;
}

void pmm_pfaFreeSinglePage(uintptr_t address) {
	pmm_ShimFreeFrame(address);
	pmm_pfaBitmapClearFrame(address);
}

void pmm_pfaSetMaxMemory(uintptr_t max) {
	mem_end = max;
}

inline uintptr_t pmm_pfaAllocateFirst() {
	uintptr_t address = pmm_pfaBitmapGetFirstFrame();
	pmm_pfaAllocateSinglePage(address);
	return address;
}

void pmm_pfaInit() {
	//printf("Allocating pages...\n");
	mem_end_aligned = (mem_end & 0xFFFFF000);

	frame_amount = mem_end_aligned / 4;
	bitmap = early_malloc(INDEX_FROM_BIT(frame_amount)*4);
	memset(bitmap, 0, INDEX_FROM_BIT(frame_amount)); //Clear frame

	#ifdef DEBUG
	int amm_alloc_mb = ((mem_end_aligned/1024)/1024);
	printk(LOG_DEBUG,"pmm","Can allocate for 0x%X (~%d MB) of ram\n",mem_end_aligned,amm_alloc_mb);
	printk(LOG_DEBUG,"pmm","Allocatable frames: 0x%X\n",frame_amount);
	printk(LOG_DEBUG,"pmm","Bitmap @ 0x%X => 0x%X\n",(uintptr_t)bitmap,(uintptr_t)bitmap + (frame_amount)/8);
	#endif
	printk(LOG_COMPLETE,"pmm","Started with no errors\n");
}
