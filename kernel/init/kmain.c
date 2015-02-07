#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;
void vmm_init();
void timing_system_engine_reportstatustoconsole();
int kmain() {
	printk(LOG_INFO,"main","Executing common code\n");
	init_early_malloc(&_kernel_end);
	init_pmm();
	vmm_init();
	heap_init();
	   uint32_t *ptr = (uint32_t*)0xA0000000;
	   uint32_t do_page_fault = *ptr;
	   printk(LOG_FAIL,"paging","%d",do_page_fault);
	timing_system_engine_reportstatustoconsole();
	while(1) {
		
	}
	return 0;
}
