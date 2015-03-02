#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <cedille/logging.h>
#include <cedille/error.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
#include <arch/x86/paging.h>
#include <arch/x86/tables.h>
extern uintptr_t em_placement_addr;
extern uint32_t _kernel_start,_kernel_end;
page_directory_t * kernel_directory = NULL;
page_directory_t * current_directory = NULL;

int paging_userealalloc = 0;

int create_pagedir() {

	kernel_directory = (page_directory_t*)early_malloc_aligned(sizeof(page_directory_t));
	memset((page_directory_t*)kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;
	printk(LOG_INTERNALS,"paging","Page directory @ 0x%x\n",current_directory);
	return 0;
}

void paging_alloc_frame(page_t *page, int kernel, int rw) {
	if (page->frame != 0) {
		printk(LOG_FAIL,"paging","frame!=0\n");
		return;
	}

	uint32_t index = pmm_pfaBitmapGetFirstFrame();
	if(index == (uint32_t)-1) {
		panic("PMM is out of memory");
	}

	pmm_pfaBitmapSetFrame(index * 0x1000);

	page->present = 1;
	page->rw = (rw)?1:0; // Should the page be writeable?
	page->user = (kernel)?0:1; // Should the page be user-mode?
	page->frame = index;
}

page_t * paging_get_page(uintptr_t address, int create, page_directory_t *dir) {
	uint32_t page_index = address / 0x1000;		//Page index
	uint32_t table_index = page_index / 1024;	//Table index

	if (dir->tables[table_index]!=NULL) {
		return &dir->tables[table_index]->pages[page_index%1024];
	}
	else if(create) {
		uint32_t tmp;
		dir->tables[table_index] = (page_table_t*)early_malloc_aligned_pz(sizeof(page_table_t),&tmp); //get physical address , &tmp
		memset(dir->tables[table_index], 0, 0x1000);
		dir->tablesPhysical[table_index] = tmp | 0x7; // PRESENT, RW, US.
		return &dir->tables[table_index]->pages[address%1024];
	}
	return NULL;
}

void paging_free_frame(page_t *page) {
	if(!(page->frame)) {return;}
	pmm_pfaBitmapClearFrame(page->frame * 0x1000);
	page->frame = 0;
}

void paging_page_fault_handler(registers_t* regs) {
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	// The error code gives us details of what happened.
	int present   = regs->err_code & 0x1; 		// Page not present
	int rw = regs->err_code & 0x2;           	// Write operation?
	int us = regs->err_code & 0x4;           	// Processor was in user-mode?
	int reserved = regs->err_code & 0x8;     	// Overwritten CPU-reserved bits of page entry?
	int id = regs->err_code & 0x10;          	// Caused by an instruction fetch?
	printk(LOG_FAIL,"paging","Page fault at 0x%x ( ",faulting_address);
	if (rw) {printf("a write; ");}
	if (!rw) {printf("a read; ");}
	if (present) {printf("present ");} else {printf("absent ");}
	if (us) {printf("user-mode ");}
	if (reserved) {printf("reserved ");}
	if (id) {printf("instruction fetch ");}
	if (faulting_address < (uint32_t)&_kernel_end) {printf("[in kernel] ");}
	printf(")\n");
	panic("PAGEFAULT");
}

int paging_setup() {
	create_pagedir();
	uintptr_t i = 0;
	while (i < em_placement_addr)
	{
		// Kernel code is readable but not writeable from userspace.
		paging_alloc_frame( paging_get_page(i, 1, current_directory), 1, 0);

		i += 0x1000;
	}
	register_interrupt_handler (14, paging_page_fault_handler);

	asm volatile("mov %0, %%cr3":: "b"(&current_directory->tablesPhysical));
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));

	return 0;
}


