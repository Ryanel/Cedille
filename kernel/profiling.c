#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <state.h>
#include <logging.h>
#include <error.h>
#include <text_console.h>

#define PROFILER_COLOR_KERNELMEM 0xA
#define PROFILER_COLOR_KERNELRES 0xC
#define PROFILER_COLOR_KERNELUNK 0x7
extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;
extern uintptr_t * em_placement_addr;
extern uintptr_t * em_placement_addr_original;

//Assumes memory is contiguous
void profile_kmemory()
{
	uintptr_t start = (uintptr_t)&_kernel_start;
	uintptr_t end   = ((uintptr_t)&_kernel_end - (uintptr_t)&_kernel_start) + ((uintptr_t)em_placement_addr - (uintptr_t)em_placement_addr_original);
	
	uintptr_t scale = end / 80;
	
	uintptr_t isdrawn_KernelImage = 0;
	uintptr_t isdrawn_PreAllocRes = 0;
	uintptr_t isdrawn_Unknown = 0;
	uintptr_t isdrawn_Heap = 0;
	
	printk(LOG_DEBUG,"memprof","Starting display of kernel memory...\n");
	for(int x = 0; x < 80; x++)
	{
		if(((x * scale)<=(uintptr_t)&_kernel_end - (uintptr_t)&_kernel_start) && ((x * scale)>=(uintptr_t)&_kernel_start) - start) // Kernel memory
		{
			text_console_change_color(PROFILER_COLOR_KERNELMEM);
			isdrawn_KernelImage = 1;
		}
		else if(((x * scale)<=(uintptr_t)em_placement_addr - (uintptr_t)em_placement_addr_original) && ((x * scale)>=(uintptr_t)em_placement_addr_original) - (uintptr_t)&_kernel_end) // Non heap allocated
		{
			text_console_change_color(PROFILER_COLOR_KERNELRES);
			isdrawn_PreAllocRes = 1;
		}
		else
		{
			isdrawn_Unknown = 1;
			text_console_change_color(PROFILER_COLOR_KERNELUNK);
		}
		printf("%c",219);
	}
	if(isdrawn_KernelImage)
	{
		text_console_change_color(PROFILER_COLOR_KERNELMEM);
		printf(" * Kernel Image (from ELF)\n");
	}
	if(isdrawn_PreAllocRes)
	{
		text_console_change_color(PROFILER_COLOR_KERNELRES);
		printf(" * Allocated Resources (before heap) [Nonfreeable]\n");
	}
	if(isdrawn_Unknown)
	{
		text_console_change_color(PROFILER_COLOR_KERNELUNK);
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
