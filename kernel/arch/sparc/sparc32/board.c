#include <stdint.h>
#include <arch/sparc/openprom.h>
extern prom_vec_t * prom_vec;
void log_low(char * s) {

}
void text_console_printchar(char c, uint8_t x, uint8_t y) {
    //use openfirmware to print a char
    prom_vec->pv_nbputchar('h');
}