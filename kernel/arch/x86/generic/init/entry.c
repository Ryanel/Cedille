#include <stddef.h>
#include <stdint.h>

#include <state.h>
#include <logging.h>
#include <error.h>

uint32_t x86_init_descriptor_tables();

int kernel_entry (void) {
    text_console_init();
    low_printname();
	log_low("Using advanced logging...\n\n");
    printk("status","Entering Boot Phase\n");
    printk("cpu","Initialising hardware\n");
	x86_init_descriptor_tables();
	printk("cpu","Initialised GDT, TSS, IDT, and ISR's\n");
	printk("status","Ending Boot Phase...\n");
	oops("Test oops");
    idle();
    return 0;
}