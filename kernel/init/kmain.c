#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;
void vmm_init();
void timing_system_engine_reportstatustoconsole();

int kernel_idle_process() {
	printk(LOG_INFO,"idle","Kernel is done initialising! Idling...\n");
	for(;;);
}

int kmain() {
	printk(LOG_INFO,"main","Executing common code\n");
	init_early_malloc(&_kernel_end);
	init_pmm();
	vmm_init();
	heap_init();
	timing_system_engine_reportstatustoconsole();
	//TODO: Setup a process system, set the kernel as a process, then immediately jump to kernel_idle_process();3
	kernel_idle_process();
	return 0;
}
