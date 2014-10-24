#include <state.h>
#include <logging.h>

int kernel_entry (void) {
	for(int i = 0; i!=80; i++) {
		log_low("-");
	}
	log_low("\n");
	text_console_init();
	low_printname();
	printk("status","Logging on UART0!\n");
	printk("status","Entering boot\n");
	idle();
    return 0;
}