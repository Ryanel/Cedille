#include <logging.h>
#include <vfs.h>

void kmain()
{
	printk("ok","Entering Kernel Enviorment...\n");	
	if(init_vfs()!=0)
	{
		printk("fail","kvfsd failed to start. Halting...\n");
		return;
	}
	printk("ok","Entering idle loop...\n");
	for(;;)
	{

	}
}