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
void pmm_set_frame(uint32_t address);
///Set the frame that governs address to free
void pmm_clear_frame(uint32_t address);
///Is the frame that governs address used?
uint32_t pmm_test_frame(uint32_t address);
///First free frame
uint32_t pmm_first_frame();
///How many possible frames?
uint32_t pmm_frame_amount();
///Allocate a frame from page
void pmm_alloc_frame(uint32_t address, int kernel, int rw);
void pmm_shim_alloc_frame(uint32_t address, int kernel, int rw);
///Free a frame
void pmm_free_frame(uint32_t address);
void pmm_shim_free_frame(uint32_t address);
#endif