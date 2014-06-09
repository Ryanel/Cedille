#include <stddef.h>
#include <stdint.h>
#include <logging.h>
#include <console.h>
#include <stdbool.h>
#include <cedille.h>
#include <multiboot.h>
#include <memory.h>
extern uint32_t _kernel_start,_kernel_end;
int e = 0x013;
int x86_init_descriptor_tables();
void pit_install(uint32_t frequency);
///The entry point for the x86 version of the Cedille Microkernel
void kernel_entry(int magic, multiboot_info_t * multiboot)
{
	console_init();
	console_printdiv();

	printk("ok","The Cedille Microkernel v.%s. (c) Corwin McKnight 2014\n",CEDILLE_VERSION_S);
	printk("info","kernel image(ram): 0x%X - 0x%X (%d bytes)\n",&_kernel_start,&_kernel_end, &_kernel_end - &_kernel_start);
	
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
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
	init_malloc(0);
	init_pmm(multiboot->mem_upper);

	return;
}

void kernel_atexit()
{
	printk("info","Cedille has nothing left to do - and we stopped spinning\n");
	printk("ok","\\==> Cedille is now halting!\n");
	console_printdiv();
	printk("info","This computer is now halted. You can do nothing but shut it down now\n");
	console_printdiv();
}