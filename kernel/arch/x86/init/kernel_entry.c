#include <stddef.h>
#include <stdint.h>
#include <logging.h>
#include <console.h>
#include <stdbool.h>
#include <cedille.h>
extern uint32_t _kernel_start,_kernel_end;

int x86_init_descriptor_tables();

///The entry point for the x86 version of the Cedille Microkernel
void kernel_entry(int magic)
{
	console_init();
	printk("ok","The Cedille Microkernel v.%s. (c) Corwin McKnight 2014\n",CEDILLE_VERSION_S);
	printk("info","kernel image(ram): 0x%X - 0x%X (%d bytes)\n",&_kernel_start,&_kernel_end, &_kernel_end - &_kernel_start);
	if(magic != 0x2BADB002)
	{
		printk("fail","Cedille was booted improperly by the bootloader\n");
		printk("info","\\==> Kernel is now halting\n");
		return;
	}
	printk("status","Initialising the processor...\n");
	x86_init_descriptor_tables();
	printk("ok","Installed Descriptor Tables\n");
	printk("cpu","Starting interrupts...\n");
	asm("sti");
	printk("device","Starting (basic) PIT...\n");
	pit_install(1000);
	while(true)
	{
		//Do nothing
	}
	return;
}

void kernel_atexit()
{
	printk("info","Cedille has nothing left to do - and we stopped spinning\n");
	printk("ok","\\==> Cedille is now halting!\n");
}