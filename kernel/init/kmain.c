#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;
void vmm_init();
void kprocess_init();
void timing_system_engine_reportstatustoconsole();

int kernel_idle_process() {
	timing_system_engine_reportstatustoconsole();
	printk(LOG_INFO,"idle","Kernel is done initialising! Idling...\n");
	for(;;);
}

int kmain() {
	printk(LOG_INFO,"main","Executing common code\n");

	low_printname();

	init_early_malloc(&_kernel_end);
	init_pmm();
	vmm_init();
	heap_init();
	kernel_idle_process();
	return 0;
}
