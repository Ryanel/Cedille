#include <stdint.h>
#include <logging.h>
uint32_t oops_counter = 0;
void processor_halt();
void halt() {
	printk("state","Halting...\n");
	processor_halt();
}

void oops(char * s) {
	oops_counter++;
	printk("oops","kernel[oops#%02d]-> \"%s\"\n",oops_counter,s);
}

void panic(char * s) {
	oops_counter++;
	printk("oops","kernel panic-> \"%s\"\n",s);
	halt();
}