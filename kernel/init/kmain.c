#include <logging.h>
#include <cedille/pmm.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;

void vmm_init();
void kprocess_init();
void timing_system_engine_reportstatustoconsole();
void kernel_doperiodic(int force, int tick);

int kernel_idle_process() {
	int i = 0;
	for(;;) {
		// Attempt to reschedule other processes unless things need to be done
		// So, check if theres anything that needs to be done (like kernel state verification)
		// Execute that, make sure everything is preemptable
		// Then, attempt to reschedule processes if there isn't anything.
		// Otherwise cpu time is wasted.
		if(i != 500) {
			printf("test%d\n",i++);
		}
	}
	return 0;
}

int kmain() {
	low_printname();

	init_early_malloc(&_kernel_end);
	init_pmm();
	vmm_init();
	heap_init();

	timing_system_engine_reportstatustoconsole();

	kernel_doperiodic(1,0); // Force stuff to happen once at the end of initialisation

	kernel_idle_process();
	return 0;
}
