#include <stdint.h>
#include <logging.h>
uint32_t oops_counter = 0;
void processor_halt();
void halt() {
	printk(LOG_FAIL,"halt","Halting...\n");
	processor_halt();
}

void oops(char * s) {
	oops_counter++;
	printk(LOG_FAIL,"oops","[oops#%02d]=> \"%s\"\n",oops_counter,s);
}

void panic(char * s) {
	oops_counter++;
	printk(LOG_FAIL,"panic","Panic! \"%s\"\n",s);
	halt();
}
