#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <state.h>
#include <logging.h>
#include <error.h>
#include <text_console.h>
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uintptr_t * em_placement_addr;
extern uintptr_t * em_placement_addr_original;

//Assumes memory is contiguous
void profile_kmemory()
{
	uint32_t start = (uint32_t)&_kernel_start;
	uint32_t end   = ((uint32_t)&_kernel_end - (uint32_t)&_kernel_start) + ((uint32_t)em_placement_addr - (uint32_t)em_placement_addr_original);
	
	uint32_t scale = end / 80;
	
	uint32_t isdrawn_KernelImage = 0;
	uint32_t isdrawn_PreAllocRes = 0;
	uint32_t isdrawn_Unknown = 0;
	uint32_t isdrawn_Heap = 0;
	
	printk("debug","Starting display of kernel memory...\n");
	for(int x = 0; x < 80; x++)
	{
		if(((x * scale)<=(uint32_t)&_kernel_end - (uint32_t)&_kernel_start) && ((x * scale)>=(uint32_t)&_kernel_start) - start) // Kernel memory
		{
			text_console_change_color(0xA);
			isdrawn_KernelImage = 1;
		}
		else if(((x * scale)<=(uint32_t)em_placement_addr - (uint32_t)em_placement_addr_original) && ((x * scale)>=(uint32_t)em_placement_addr_original) - (uint32_t)&_kernel_end) // Non heap allocated
		{
			text_console_change_color(0xC);
			isdrawn_PreAllocRes = 1;
		}
		else
		{
			isdrawn_Unknown = 1;
			text_console_change_color(0x7);
		}
		printf("%c",219);
	}
	if(isdrawn_KernelImage)
	{
		text_console_change_color(0xA);
		printf(" * Kernel Image (from ELF)\n");
	}
	if(isdrawn_PreAllocRes)
	{
		text_console_change_color(0xC);
		printf(" * Allocated Resources (before heap) [Nonfreeable]\n");
	}
	if(isdrawn_Unknown)
	{
		text_console_change_color(0x7);
		printf(" * Unknown (in Kernel Address Space)\n");
	}
	if(isdrawn_Heap)
	{
		text_console_change_color(0xB);
		printf(" * Kernel Heap\n");
	}
	
	
	text_console_change_color(0xF);
	printf("+------------------------------------------------------------------------------+");
}