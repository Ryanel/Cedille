#include <logging.h>
void kmain()
{
	printk("debug","Entering Kernel Enviorment...\n");
	profiler_memory();
}