#include <stddef.h>
#include <stdint.h>
void arm_integratorcp_driver_serial_puts (const char * str);
void arm_integratorcp_driver_serial_putc (char c);

void text_console_printchar(char c, uint8_t x, uint8_t y) {
    arm_integratorcp_driver_serial_putc(c);
}
void log_low(const char * str) {
    arm_integratorcp_driver_serial_puts(str);
}


void text_console_change_color(uint8_t color) {
}

void text_console_reset_color() {
}

void text_console_scroll(int x, int x2) {
}

void text_console_setcursor(int x, int y) {
}