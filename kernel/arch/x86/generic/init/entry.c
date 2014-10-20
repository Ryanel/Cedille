#include <stddef.h>
#include <state.h>
#include <logging.h>
#include <stdint.h>

uint32_t x86_init_descriptor_tables();

int kernel_entry (void) {
    text_console_init();
    low_printname();
    printk("status","Entering Boot Phase\n");
    printk("cpu","Initialising hardware\n");
	x86_init_descriptor_tables();
	printk("cpu","Initialised GDT, TSS, IDT, and ISR's\n");
	printk("status","Ending Boot Phase...\n");
    idle();
    return 0;
}