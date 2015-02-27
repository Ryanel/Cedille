#include <stdint.h>
#include <logging.h>
uint32_t oops_counter = 0;
void processor_halt();
void kernel_doperiodic(int force, int tick);
void halt() {
	printk(LOG_FAIL,"halt","Halting...\n");
	kernel_doperiodic(1,0);
	processor_halt();
}

void oops(char * s) {
	oops_counter++;
	printk(LOG_FAIL,"oops","oops %02d: \"%s\"\n",oops_counter,s);
	kernel_doperiodic(1,0);
}

void panic(char * s) {
	oops_counter++;
	printk(LOG_FAIL,"panic","%s\n",s);
	kernel_doperiodic(1,0);
	halt();
}
