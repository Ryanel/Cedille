#include <stddef.h>
#include <stdint.h>

#include <state.h>
#include <logging.h>
#include <error.h>
#include <text_console.h>

#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;

uint32_t x86_init_descriptor_tables();
void pit_install(uint32_t frequency);

int kernel_entry (void) {
	
    text_console_init();
    low_printname();

	log_low("Using advanced logging...\n\n");
	
    printk("status","Entering boot phase\n");
    printk("info","Initialising hardware\n");
	x86_init_descriptor_tables();
	printk("cpu","Initialised basic hardware tables\n");
	
	//Initialise PIT so interrupt handler can shut up
	pit_install(1000);
	
	asm("sti"); // Start interrupts
	
	printk("info","Initialising physical memory manager.\n");
	init_early_malloc(&_kernel_end);
	init_pmm();
	
	printk("status","Ending Boot Phase...\n");
	while(1) {
	}
    idle();
    return 0;
}