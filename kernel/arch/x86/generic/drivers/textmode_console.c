#include <stddef.h>
#include <stdint.h>

#define X86_CONSOLE_WIDTH  80
#define X86_CONSOLE_HEIGHT 24

#define X86_CONSOLE_COLOR 0x0F

volatile uint8_t *textmode_buffer=(uint8_t*)0xB8000;

void arch_x86_driver_textmode_writechar(char c,uint8_t attribute, uint8_t x, uint8_t y) {
    textmode_buffer[((y * X86_CONSOLE_WIDTH) + x) * 2] = c;
    textmode_buffer[(((y * X86_CONSOLE_WIDTH) + x) * 2) + 1] = attribute;
}


void text_console_printchar(char c, uint8_t x, uint8_t y) {
    arch_x86_driver_textmode_writechar(c,X86_CONSOLE_COLOR, x, y);
}