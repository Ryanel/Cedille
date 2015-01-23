#include <stdint.h>
#include <stddef.h>

uintptr_t * early_malloc_aligned(uint32_t sz);

uintptr_t * page_directory;

uint32_t first_page_table[1024] __attribute__((aligned(4096)));
int create_pagedir() {
	// Todo, actually dynamically create this from info from PMM. For now, using osdev to get a working interface working
	page_directory = early_malloc_aligned(0x1000 * 4); // Allocate page size times bytes per int (4)
	for(int i = 0; i < 1024; i++) {
	    // This sets the following flags to the pages:
	    //   Supervisor: Only kernel-mode can access them
	    //   Write Enabled: It can be both read from and written to
	    //   Not Present: The page table is not present
	    page_directory[i] = 0x00000002;
	}
	page_directory[1023] = (unsigned int)page_directory | 3; // Map it to iself
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