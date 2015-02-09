#include <stdint.h>
#include <arch/sparc/openprom.h>
extern prom_vec_t * prom_vec;
void log_low(char * s) {

}
void text_console_printchar(char c, uint8_t x, uint8_t y) {
    //use openfirmware to print a char
    prom_vec->pv_nbputchar('h');
}


void text_console_change_color(uint8_t color) {

}

void text_console_reset_color() {

}

void text_console_setcursor(uint8_t x,uint8_t y)
{

}
void text_console_scroll(int from,int to) {

}

int vmm_shim_doBoardSetup() {
	return -1;
}

void processor_halt() {
	for(;;);
}
