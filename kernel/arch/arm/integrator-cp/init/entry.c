#include <state.h>
#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
#include <arch/arm/arm4/interrupts.h>
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
#include <arch/arm/arm4/integrator-cp/pl110.h>
#endif

void kmain();

extern uintptr_t _kernel_start,_kernel_end;

int kernel_entry (void) {
	text_console_init();
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
	arm4_integrator_cp_pl110_start();
	printk("boot","Started graphics device \"PL110\"\n");
	printk("","===> Warning! PL110 support is supplimental! NORMAL OUTPUT IS ON SERIAL\r\n");
#endif
	
	for(int i = 0; i!=80; i++) {
		log_low("-");
	}
	log_low("\n");
	low_printname();
	printk("debug","Kernel Resides @ 0x%X => 0x%X\n",&_kernel_start,&_kernel_end);
	printk("debug","Logging on UART0!\n");
    printk("status","Entering boot phase\n");
    printk("info","Initialising hardware\n");
	printk("cpu","arm4 => Initialising Interrupts...\n");

	arm4_init_interrupts();
	asm("swi 0");
	
	printk("info","Initialising physical memory manager.\n");
	pmm_set_maxmem(0x8000000);
	
	kmain();
    return 0;
}
