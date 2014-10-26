#include <state.h>
#include <logging.h>

int kernel_entry (void) {
	for(int i = 0; i!=80; i++) {
		log_low("-");
	}
	log_low("\n");
	text_console_init();
	low_printname();
	log_low("Using advanced logging...\n\n");
	printk("status","Logging on UART0!\n");
    printk("status","Entering boot phase\n");
    printk("cpu","Initialising hardware\n");
	//
	printk("cpu","Initialised basic hardware tables\n");
	printk("status","Ending Boot Phase...\n");
	idle();
    return 0;
}