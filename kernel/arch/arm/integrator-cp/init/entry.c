#include <cedille/state.h>
#include <cedille/logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
#include <arch/arm/arm4/interrupts.h>
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
#include <arch/arm/arm4/integrator-cp/pl110.h>
#endif

int kmain();

extern uintptr_t _kernel_start,_kernel_end;

int kernel_entry (void) {
	text_console_init();
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
	arm4_integrator_cp_pl110_start();
	printk(LOG_INFO,"pl110","Started graphics device \"PL110\"\n");
	printk(LOG_WARN,"pl110","Warning! PL110 support is supplimental! NORMAL OUTPUT IS ON SERIAL\r\n");
#endif
	kprocess_init();
	printk(LOG_DEBUG,"mem","Kernel Resides @ 0x%X => 0x%X\n",&_kernel_start,&_kernel_end);
	printk(LOG_INFO,"uart","Logging on UART0!\n");
	printk(LOG_INFO,"arm4interrupt","Initialising Interrupts...\n");

	//arm4_init_interrupts();
	//asm("swi 0");
	
	printk(LOG_INFO,"pmm","Initialising physical memory manager.\n");
	pmm_set_maxmem(0x8000000);
	
	kmain();
    return 0;
}
