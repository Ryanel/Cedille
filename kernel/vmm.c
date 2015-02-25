#include <stdint.h>
#include <stddef.h>
#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/vmm.h>
#include <assert.h>
void vmm_shim_doBoardSetup();

void vmm_init() {
	vmm_shim_doBoardSetup();
	printk(LOG_COMPLETE,"vmm","Board init completed with no errors\n");
	vmm_doNormalSetup();
	printk(LOG_COMPLETE,"vmm","Started with no errors\n");
}

void vmm_doNormalSetup() {
	// Init vmm structures and setup the heap space.
}

uintptr_t vmm_map_addresses(vmm_mapping_entry_t * entry) {
	//TODO: vmm_shim_mapToPhys()
	return 0;
}

void vmm_map_entry(vmm_mapping_entry_t * entry) {
	printk(LOG_DEBUG,"vmm","Mapping entry 0x%x!\n",entry);
	// Request phys memory from PMM
	assert(((uintptr_t)entry->virt != 0 && (uintptr_t)entry->virtend != 0));
	//Okay, now ask it to be mapped to this address.
}

void vmm_unmap_entry(vmm_mapping_entry_t * entry) {

}