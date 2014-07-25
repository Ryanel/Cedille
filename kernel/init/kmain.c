#include <logging.h>
#include <stdio.h>
#include <vfs.h>
#include <cedille.h>

void profiler_memory();

int start_service(char *daemon,int essential,int (*func)())
{
	printk("daemon","Starting kernel daemon %s...",daemon);
	if((*func)==0)
	{
		printf("\r\t\t\t\t\t\t\t\t\t  [FAIL]");
		if(essential)
		{
			panic("Couldn't start required daemon!");
		}
		return 1;
	}
	else
	{
		printf("\r\t\t\t\t\t\t\t\t\t  [ OK ]");
	}
	return 0;
}

int init_timer()
{
	return 0;
}

void kmain()
{
	printk("ok","Entering Kernel Enviorment...\n");	

	//Start services
	
	start_service("kvfsd",1,init_vfs);
	start_service("timerd",1,init_timer);
	
	printk("login","Login unimplemented; idling main kernel thread\n");
	//profiler_memory();
	for(;;)
	{

	}
}
