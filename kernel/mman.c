#include <stdint.h>
#include <stddef.h>
extern uint32_t _kernel_end;
uintptr_t pre_placementaddr;
uint32_t kmalloc_status = 0;

void init_malloc(uint32_t status)
{
	if(status == 0)
	{
		pre_placementaddr = &_kernel_end;
		kmalloc_status = 0;
	}
}

uintptr_t * kmalloc(size_t sz)
{
	uint32_t tmp = pre_placementaddr;
	pre_placementaddr += sz;
	return (uintptr_t *)tmp;
}
uintptr_t * kmalloc_aligned(size_t sz)
{
	pre_placementaddr &= 0xFFFFF000;
	pre_placementaddr += 0x1000;
	uint32_t tmp = pre_placementaddr;
	pre_placementaddr += sz;
	return (uintptr_t *)tmp;
}
uintptr_t * kmalloc_aligned_phys(size_t sz, uintptr_t * phys)
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
uintptr_t mman_get_placement_address()
{
	return pre_placementaddr;
}