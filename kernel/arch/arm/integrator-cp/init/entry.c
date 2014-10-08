#include <state.h>
#include <logging.h>

int kernel_entry (void) {
	for(int i = 0; i!=80; i++) {
		log_low("-");
	}
	log_low("\n");
	text_console_init();
	low_printname();
	log(LOG_KERNEL,"Logging on UART0!\n");
	log(LOG_STATECHANGE,"Entering Boot Phase\n");
	idle();
    return 0;
}