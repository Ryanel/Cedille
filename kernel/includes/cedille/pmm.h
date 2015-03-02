#include <stdint.h>
#include <stddef.h>

#ifndef PMM_H
#define PMM_H

///Page index from address
#define INDEX_FROM_BIT(b) (b / 0x20)
///Page offest from address
#define OFFSET_FROM_BIT(b) (b % 0x20)

///Init the pmm
void pmm_pfaInit();
///Set the frame that governs address to used
void pmm_pfaBitmapSetFrame(uintptr_t address);
///Set the frame that governs address to free
void pmm_pfaBitmapClearFrame(uintptr_t address);
///Is the frame that governs address used?
uintptr_t pmm_pfaBitmapTestFrame(uintptr_t address);
///First free frame
uintptr_t pmm_pfaBitmapGetFirstFrame();
///How many possible frames?
uintptr_t pmm_pfaGetTotalPages();
///Allocate a single frame
void pmm_pfaAllocateSinglePage(uintptr_t address);
///Free a frame
void pmm_pfaFreeSinglePage(uintptr_t address);
void pmm_pfaSetMaxMemory(uintptr_t max);

uintptr_t * pmm_pfaAllocatePages(unsigned int n);

void pmm_ShimAllocFrame(uintptr_t address, int kernel, int rw);
void pmm_ShimFreeFrame(uintptr_t address);
#endif
