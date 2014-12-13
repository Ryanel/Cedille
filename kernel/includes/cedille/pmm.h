#include <stdint.h>
#include <stddef.h>

#ifndef PMM_H
#define PMM_H
///Page index from address
#define INDEX_FROM_BIT(b) (b / 0x20)
///Page offest from address
#define OFFSET_FROM_BIT(b) (b % 0x20)
///Init the pmm
void init_pmm();
///Set the frame that governs address to used
void pmm_set_frame(uintptr_t address);
///Set the frame that governs address to free
void pmm_clear_frame(uintptr_t address);
///Is the frame that governs address used?
uintptr_t pmm_test_frame(uintptr_t address);
///First free frame
uintptr_t pmm_first_frame();
///How many possible frames?
uintptr_t pmm_frame_amount();
///Allocate a frame from page
void pmm_alloc_frame(uintptr_t address, int kernel, int rw);
void pmm_shim_alloc_frame(uintptr_t address, int kernel, int rw);
///Free a frame
void pmm_free_frame(uintptr_t address);
void pmm_shim_free_frame(uintptr_t address);

void pmm_set_maxmem(uintptr_t max);
#endif