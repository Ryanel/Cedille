#include <stdint.h>
#include <assert.h>
uint32_t cedille_criticalsection_semaphore = 1;

void cedille_enter_criticalsection() {
	assert(cedille_criticalsection_semaphore == 1);
	cedille_criticalsection_semaphore = 0;
	return;
}

void cedille_exit_criticalsection() {
	assert(cedille_criticalsection_semaphore == 0);
	cedille_criticalsection_semaphore = 1;
	return;
}
