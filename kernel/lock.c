#include <stdint.h>
#include <assert.h>
uint32_t kernel_criticalsection_semaphore = 1;

void kernel_enter_criticalsection() {
	assert(kernel_criticalsection_semaphore >= 1);
	kernel_criticalsection_semaphore = 0;
	return;
}

void kernel_exit_criticalsection() {
	assert(kernel_criticalsection_semaphore == 0);
	kernel_criticalsection_semaphore = 1;
	return;
}
