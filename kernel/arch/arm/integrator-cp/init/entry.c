#include <state.h>
void arm_integratorcp_driver_serial_puts (const char * str);
void log_low(const char * str) {
	arm_integratorcp_driver_serial_puts(str);
}

int kernel_entry (void) {
	for(int i = 0; i!=80; i++) {
		log_low("-");
	}
	log_low("\n");
	log_low("Logging on UART0!\n");
	log_low("The Cedille Kernel - v. 0.0.0.7-rw\n");
	log_low("*** Entering Boot Phase [1/2] ***\n");
	log_low("*** Switching to task [idle] ***");
	idle();
    return 0;
}