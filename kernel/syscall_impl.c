#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <logging.h>
#include <stdio.h>
#include <cedille.h>
#include <syscall.h>
//void register_syscall_handler (uint8_t n, syscall_handler_t h);
//void deregister_syscall_handler (uint8_t n);
int syscall_panic(char * s)
{
	panic(s);
	return 0;
}

int syscall_oops(char * s)
{
	oops(s);
	return 0;
}

void init_syscall()
{
	register_syscall_handler (0x0, &syscall_panic);
	register_syscall_handler (0x1, &syscall_oops);
}