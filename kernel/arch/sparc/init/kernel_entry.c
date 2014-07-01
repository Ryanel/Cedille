#include <stdint.h>
#include <stddef.h>
#include <arch/sparc/openprom.h>
#include <arch/sparc/serial.h>
prom_vec_t * prom_vec = NULL;

void serial_init();

void printc(char c)
{
	prom_vec->pv_nbputchar(c);
}

void kernel_entry(prom_vec_t * pv)
{
	prom_vec = pv;
	printc('#');
}