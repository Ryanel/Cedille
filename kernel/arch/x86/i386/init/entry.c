#include <stddef.h>
#include <stdint.h>

#include <state.h>
#include <logging.h>
#include <error.h>
#include <text_console.h>

#include <cedille/pmm.h>
#include <cedille/heap.h>
#include <arch/x86/tables.h>
extern uint32_t _kernel_start,_kernel_end;

uint32_t x86_init_descriptor_tables();
void pit_install(uint32_t frequency);
void irq1fix();
void profile_kmemory();

int kernel_entry (void) {
	
    text_console_init();
    low_printname();
	printk("debug","Kernel Resides @ 0x%X => 0x%X\n",&_kernel_start,&_kernel_end);
    printk("status","Entering boot phase\n");
    printk("info","Initialising hardware\n");
	x86_init_descriptor_tables();
	printk("cpu","Initialised basic hardware tables\n");
	
	//Initialise PIT so interrupt handler can shut up
	pit_install(1000);
	register_interrupt_handler (IRQ1, irq1fix); //Temp IRQ1 fix
	asm("sti"); // Start interrupts
	
	printk("info","Initialising physical memory manager.\n");
	init_early_malloc(&_kernel_end);
	init_pmm();
	
	printk("status","Ending Boot Phase...\n");
#ifdef DEBUG
	profile_kmemory();
#endif
	while(1) {
	}
    idle();
    return 0;
}