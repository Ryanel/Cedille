/*
 * paging.h
 */

#ifndef KERNEL_INCLUDES_ARCH_X86_PAGING_H
#define KERNEL_INCLUDES_ARCH_X86_PAGING_H

typedef struct page {
	uint32_t present    : 1;
	uint32_t rw         : 1;
	uint32_t user       : 1;
	uint32_t accessed   : 1;
	uint32_t dirty      : 1;
	uint32_t unused     : 7;
	uint32_t frame      : 20;
} page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

typedef struct page_directory {
   page_table_t *tables[1024];

   uint32_t tablesPhysical[1024];

   uint32_t physicalAddr;
} page_directory_t;

void paging_alloc_frame(page_t *page, int kernel, int rw);
page_t * paging_get_page(uintptr_t addr, int create, page_directory_t *dir);
void paging_free_frame(page_t *page);
int paging_setup();
#endif /* KERNEL_INCLUDES_ARCH_X86_PAGING_H */
