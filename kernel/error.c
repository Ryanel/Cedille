#include <stdint.h>
#include <logging.h>
uint32_t oops_counter = 0;
void oops(char * s) {
	oops_counter++;
	printk("oops","kernel[oops#%02d]-> \"%s\"",oops_counter,s);
}