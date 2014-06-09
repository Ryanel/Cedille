#include <stdint.h>
#include <stddef.h>
extern uint32_t _kernel_end;
uintptr_t pre_placementaddr;
uint32_t kmalloc_status = 0;
void init_malloc(uint32_t status)
{
	if(status == 0)
	{
		pre_placementaddr = _kernel_end;
		kmalloc_status = 0;
	}
}

uintptr_t * kmalloc(size_t sz)
{
	uint32_t tmp = pre_placementaddr;
	pre_placementaddr += sz;
	return (uintptr_t *)tmp;
}