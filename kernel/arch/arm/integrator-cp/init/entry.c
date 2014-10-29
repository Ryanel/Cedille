#include <state.h>
#include <logging.h>
#include <arch/arm/arm4/interrupts.h>
#ifdef GRAPHICS_DEVICE_PL110
#include <arch/arm/arm4/integrator-cp/pl110.h>
#endif
int kernel_entry (void) {
#ifdef GRAPHICS_DEVICE_PL110
	arm4_integrator_cp_pl110_start();
	log_low("kernel[pre]-> Started graphics device \"PL110\"\n");
#endif
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
	printk("arm4","Initialising Interrupts...\n");
	arm4_init_interrupts();
	asm("swi 0");
	printk("status","Ending Boot Phase...\n");
	
	idle();
    return 0;
}