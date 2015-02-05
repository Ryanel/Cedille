#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;
void init_vmm();
void timing_system_engine_reportstatustoconsole();
int kmain() {
	printk("info","kernel[main]-> Executing common code\n");
	init_early_malloc(&_kernel_end);
	init_pmm();
	init_vmm();
	timing_system_engine_reportstatustoconsole();
	while(1) {
		
	}
	return 0;
}
