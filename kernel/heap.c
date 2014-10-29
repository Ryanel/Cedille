#include <stdint.h>
#include <stddef.h>
uintptr_t * em_placement_addr;
void init_early_malloc(uintptr_t * address) {
	em_placement_addr = address;
}

uintptr_t * early_malloc(uint32_t sz) {
	uintptr_t * tmp = em_placement_addr;
	em_placement_addr += sz;
	return tmp;
}