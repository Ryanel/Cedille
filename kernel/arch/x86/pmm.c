#include <stdint.h>
#include <logging.h>
#include <memory.h>
uintptr_t * bitmap;
uint32_t bitmap_frames = 0x0;
void init_pmm(uint32_t total_kb)
{
	printk("info","Starting Physical Memory Manager...\n");
	if(total_kb == 0)
	{
		printk("fail","Unknown amount of free memory? Reading Configuration Space...\n");
		return;
	}
	bitmap_frames = total_kb;
	bitmap = kmalloc(1);
	
}