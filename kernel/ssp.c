/*
 * ssp.c
 * GCC Stack Smashing Protector
 */

#include <stdint.h>
#include <cedille/kconfig.h>
#include <cedille/error.h>

uintptr_t __stack_chk_guard = CONFIG_SSP_GUARD_VAL;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
	panic("Stack smashing detected!");
	for(;;);
}
