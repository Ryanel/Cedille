#include <cedille/logging.h>
#include <cedille/pmm.h>
#include <lib/liballoc.h>
#include <cedille/heap.h>
extern uint32_t _kernel_start,_kernel_end;

void vmm_init();
void kprocess_init();
void timing_system_engine_reportstatustoconsole();
void kernel_doperiodic(int force, int tick);

int kernel_idle_process() {
	for(;;) {
		// Attempt to reschedule other processes unless things need to be done
		// So, check if theres anything that needs to be done (like kernel state verification)
		// Execute that, make sure everything is preemptable
		// Then, attempt to reschedule processes if there isn't anything.
		// Otherwise cpu time is wasted.
	}
	return 0;
}

int kmain() {
	low_printname();

	init_early_malloc(&_kernel_end);
	pmm_pfaInit();
	vmm_init();
	heap_init();

	timing_system_engine_reportstatustoconsole();

	//Tests
	printf("Performing tests\n");
	void * a;
	void * b;
	void * c;
	void * d;
	a = malloc(32);
	b = malloc(32);
	printf("A:0x%X\n",a);
	printf("B:0x%X\n",b);
	kernel_doperiodic(1,0); // Force stuff to happen once at the end of initialisation
	kernel_idle_process();
	return 0;
}
