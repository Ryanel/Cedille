#include <logging.h>
#include <vfs.h>
void profiler_memory();

void kmain()
{
	printk("ok","Entering Kernel Enviorment...\n");	
	if(init_vfs()!=0)
	{
		printk("fail","kvfsd failed to start. Halting...\n");
		return;
	}
	profiler_memory();
}