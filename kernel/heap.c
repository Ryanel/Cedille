#include <stdint.h>
#include <stddef.h>
#include <cedille/vmm.h>

extern uintptr_t em_placement_addr;

vmm_mapping_entry_t heap_map; // Allocate at compile to force its existance
void heap_mmInit();
void heap_init() {
	heap_mmInit();
	return; //TODO: Do nothing right now
	heap_map.owned_process 	= 0;
	heap_map.type			= VMM_MAP_TYPE_HEAP;

	uintptr_t eh_address_aligned = em_placement_addr;
	if((eh_address_aligned & 0xFFFFF000) != eh_address_aligned) {
		eh_address_aligned &= 0xFFFFF000;
		eh_address_aligned += 0x1000;
	}
	heap_map.phys 			= (uintptr_t *)eh_address_aligned;
	heap_map.physend		= (uintptr_t *)((uintptr_t)heap_map.phys + 0x1000);
	heap_map.virt = heap_map.phys;
	heap_map.virtend = heap_map.physend;
	vmm_map_entry(&heap_map);
}
