/*
 * ssp.c
 * GCC Stack Smashing Protector
 */

#include <stdint.h>
#include <error.h>
#define STACK_CHK_GUARD 0xe2d00396

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
	panic("Stack smashing detected!");
	for(;;);
}
