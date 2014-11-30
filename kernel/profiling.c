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

void profile_memory()	
{
	printk("debug","Kernel Memory:\n");
	uint32_t start = (uint32_t)&_kernel_start;
	uint32_t end   = ((uint32_t)&_kernel_end - (uint32_t)&_kernel_start) + ((uint32_t)em_placement_addr - (uint32_t)em_placement_addr_original);
	uint32_t scale = end / 80;
	int allocation_label_start = 0;
	for(int x = 0; x < 80; x++)
	{
		if(((x * scale)<=(uint32_t)&_kernel_end - (uint32_t)&_kernel_start) && ((x * scale)>=(uint32_t)&_kernel_start) - start)
		{
			text_console_change_color(0xA);
			allocation_label_start++;
		}
		else
		{
			text_console_change_color(0x1);
		}
		printf("%c",219);
	}
	text_console_change_color(0xA);
	printf("Kernel Image/");
	text_console_change_color(0xF);
	for(int x = allocation_label_start - 8; x > 0; x--)
	{
		printf(" ");
	}
	text_console_change_color(0x1);
	printf("\\A.Mem/\n");
	text_console_change_color(0xF);
}