#include <logging.h>

void profiler_memory();

void kmain()
{
	printk("ok","Entering Kernel Enviorment...\n");
	profiler_memory();
}