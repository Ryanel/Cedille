/*
 * vmm.h
 */
#ifndef KERNEL_INCLUDES_CEDILLE_VMM_H_
#define KERNEL_INCLUDES_CEDILLE_VMM_H_

#include <stdint.h>

#define FEATURE_VMM_ALIGN_4K 1

#define VMM_MAP_TYPE_FORBIDDEN 0
#define VMM_MAP_TYPE_HEAP 1

typedef struct {
	uintptr_t   type;
	uintptr_t   owned_process;
	uintptr_t * phys;
	uintptr_t * physend;
	uintptr_t * virt;
	uintptr_t * virtend;
} vmm_mapping_entry_t;

void vmm_shim_doBoardSetup();
void vmm_init();
void vmm_doNormalSetup();

void vmm_map_entry(vmm_mapping_entry_t * entry);
void vmm_unmap_entry(vmm_mapping_entry_t * entry);

#endif /* KERNEL_INCLUDES_CEDILLE_VMM_H_ */
