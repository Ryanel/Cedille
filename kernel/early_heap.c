#include <stdint.h>
#include <stddef.h>
uintptr_t em_placement_addr;
uintptr_t em_placement_addr_original;
// Bug: Does not work for larger then 32-bit addressing.
void init_early_malloc(uintptr_t address) {
	em_placement_addr = address;
	em_placement_addr_original = address;
	//Fix: Forces it to be paged aligned if it's not. See early_malloc_aligned for explanation
	if((em_placement_addr & 0xFFFFF000) != em_placement_addr) {
		em_placement_addr &= 0xFFFFF000;
		em_placement_addr += 0x1000;
	}
	
}
// Bug: Does not work for larger then 32-bit addressing.
uintptr_t * early_malloc(uint32_t sz) {
	uintptr_t * tmp = (uintptr_t *)em_placement_addr;
	em_placement_addr += sz;
	return tmp;
}
// Bug: Does not work for larger then 32-bit addressing.
uintptr_t * early_malloc_aligned(uint32_t sz) {
	// Fix   : Prevent a page-sized leak when allocating on already aligned addresses
	// Reason: Prevents a page-sized leak (unusable frame) in the edge case we allocate when em_placement_addr is already page aligned.
	//       : This is because the function would simply assume that it wasn't page aligned, re-align it (assign the same value), then add 0x1000 (4k)
	if((em_placement_addr & 0xFFFFF000) != em_placement_addr) {
		em_placement_addr &= 0xFFFFF000;
		em_placement_addr += 0x1000;
	}
	
	uintptr_t * tmp = (uintptr_t *)em_placement_addr;
	em_placement_addr += sz;
	return tmp;


