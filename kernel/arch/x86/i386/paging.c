#include <stdint.h>
#include <stddef.h>
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
int create_pagedir() {
	//Todo, actually dynamically create this from info from PMM. For now, using osdev to get a working interface working
	for(int i = 0; i < 1024; i++) {
	    // This sets the following flags to the pages:
	    //   Supervisor: Only kernel-mode can access them
	    //   Write Enabled: It can be both read from and written to
	    //   Not Present: The page table is not present
	    page_directory[i] = 0x00000002;
	}
	return 0;
}
int paging_setup() {
	create_pagedir();
	for(int i = 0; i < 1024; i++) {
	    // As the address is page aligned, it will always leave 12 bits zeroed.
	    // Those bits are used by the attributes ;)
	    first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = ((unsigned int)first_page_table) | 3;
	asm volatile("mov %0, %%cr3":: "b"(page_directory));
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));
	return 0;
}
int vmm_shim_doboardsetup() {
	int ret = paging_setup();
	return ret;
}