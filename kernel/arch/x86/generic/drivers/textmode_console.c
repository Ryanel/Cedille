#include <stddef.h>
#include <stdint.h>
#include <arch/x86/ports.h>
#include <text_console.h>
#define X86_CONSOLE_WIDTH  80
#define X86_CONSOLE_HEIGHT 24

#define X86_CONSOLE_COLOR 0x0F

volatile uint8_t *textmode_buffer=(uint8_t*)0xB8000;
volatile uint16_t *textmode_buffer_16=(uint16_t*)0xB8000;
volatile uint8_t textmode_color = X86_CONSOLE_COLOR;

void arch_x86_driver_textmode_writechar(char c,uint8_t attribute, uint8_t x, uint8_t y) {
    textmode_buffer[((y * X86_CONSOLE_WIDTH) + x) * 2] = c;
    textmode_buffer[(((y * X86_CONSOLE_WIDTH) + x) * 2) + 1] = attribute;
}


void text_console_printchar(char c, uint8_t x, uint8_t y) {
    arch_x86_driver_textmode_writechar(c,textmode_color, x, y);
}

void text_console_change_color(uint8_t color) {
	textmode_color = color;
}

void text_console_reset_color() {
	textmode_color = X86_CONSOLE_COLOR;
}

void text_console_setcursor(uint8_t x,uint8_t y)
{
	uint16_t location = y * 80 + x;
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}
void text_console_scroll(int from,int to) {
	uint16_t blank = 0x20  | (textmode_color << 8);
	int i;
	for (i = from*80; i < to*80; i++)
	{
		textmode_buffer_16[i] = textmode_buffer_16[i+80];
	}

	for (i = to*80; i < (to+1)*80; i++)
	{
		textmode_buffer_16[i] = blank;
	}
}