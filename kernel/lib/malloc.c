#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <arch/x86/paging.h>
#include <cedille.h>
#include <logging.h>
extern uint32_t _kernel_end;
uintptr_t pre_placementaddr;
uint32_t kmalloc_status = 0;
extern page_directory_t * kernel_page_directory;
void init_malloc(uint32_t status)
{
	if(status == 0)
	{
		pre_placementaddr = (uintptr_t)&_kernel_end;
		kmalloc_status = 0;
	}
}

uintptr_t * kmalloc(size_t sz)
{
	if(kmalloc_status == 0)
	{
		uint32_t tmp = pre_placementaddr;
		pre_placementaddr += sz;
		return (uintptr_t *)tmp;
	}
	return NULL;
}

uintptr_t * kmalloc_aligned(size_t sz)
{
	if(kmalloc_status == 0)
	{
		pre_placementaddr &= 0xFFFFF000;
		pre_placementaddr += 0x1000;
		uint32_t tmp = pre_placementaddr;
		pre_placementaddr += sz;
		return (uintptr_t *)tmp;
	}
	return NULL;
}
uintptr_t * kmalloc_aligned_phys(size_t sz, uintptr_t * phys)
{
	if(kmalloc_status == 0)
	{
		pre_placementaddr &= 0xFFFFF000;
    	pre_placementaddr += 0x1000;
		uint32_t tmp = pre_placementaddr;
		if (phys)
		{
   		 	*phys = pre_placementaddr;
		}
		pre_placementaddr += sz;
		return (uintptr_t *)tmp;
	}
	return NULL;
}
uintptr_t mman_get_placement_address()
{
	return pre_placementaddr;
}
