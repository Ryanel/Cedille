#include <stddef.h>
#include <stdint.h>

#include <state.h>
#include <logging.h>
#include <error.h>
#include <text_console.h>

#include <cedille/pmm.h>
#include <cedille/heap.h>
#include <arch/x86/tables.h>
#include <interface/multiboot1.h>
extern uint32_t _kernel_start,_kernel_end;

uint32_t x86_init_descriptor_tables();
void pit_install(uint32_t frequency);
void irq1fix();
void kprocess_init();
int kmain();

int kernel_entry (multiboot_info_t* mbd, unsigned int magic) {
	kprocess_init();
    text_console_init();
    
    // Do multiboot verification AFTER initialising the text console.
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    	printk(LOG_FAIL,"multiboot","Magic number verification failed, it's 0x%X\n",magic);
    	panic("Multiboot verification failed!");
    }
    // It's okay to access multiboot data structures now
    
    pmm_set_maxmem(mbd->mem_upper * 1024); // It's KB, multiply to get bytes.

    #ifdef DEBUG
	printk(LOG_DEBUG,"mem","Kernel Resides @ 0x%X => 0x%X\n",&_kernel_start,&_kernel_end);
    #endif

	x86_init_descriptor_tables();

    /*----- TODO: Make dynamic / prettier -----*/
	//Initialise PIT so interrupt handler can shut up
	pit_install(1000);
	register_interrupt_handler (IRQ1, irq1fix); //Temp IRQ1 fix
	asm("sti"); // Start interrupts
    /*----- TODO: Make dynamic / prettier -----*/
	
	kmain();
    return 0;
}
