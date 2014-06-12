#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <logging.h>
#define PAGE_SIZE 0x1000

extern page_directory_t * kernel_page_directory;

//TODO: Replace with better algorithem
uintptr_t * lheap_alloc_pages(uint32_t pages)
{
	uint32_t first_free = pmm_bitmap_findfree() * 0x1000;
	printk("info","Allocating %d pages, starting at 0x%X\n",pages,first_free);
	for(int i = 0; i < pages; i++)
	{
		printk("info","Allocating page %d\n",i);
		pmm_alloc_frame(pmm_get_page(kernel_page_directory,first_free, 1), 1,1);
	}
	return NULL;
}