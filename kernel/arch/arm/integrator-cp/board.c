#include <stddef.h>
#include <stdint.h>
void arm_integratorcp_driver_serial_puts (const char * str);
void arm_integratorcp_driver_serial_putc (char c);
void graphics_terminal_drawchar(char c, int x, int y);
void graphics_terminal_resetrgb();
void graphics_terminal_setrgb(uint8_t r,uint8_t g,uint8_t b);

void text_console_printchar(char c, uint8_t x, uint8_t y) {
    arm_integratorcp_driver_serial_putc(c);
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
	graphics_terminal_drawchar(c,x,y);
#endif
}
void log_low(const char * str) {
    arm_integratorcp_driver_serial_puts(str);
}


void text_console_change_color(uint8_t color) {

	//0x0 -> 0xF are supported
#ifdef ENABLE_GRAPHICS_DEVICE_PL110
	switch(color) {
		case 0x0:
			graphics_terminal_setrgb(0,0,0);
			break;
		case 0x1:
			graphics_terminal_setrgb(0,0,0xE0);
			break;
		case 0x2:
			graphics_terminal_setrgb(0,0xE0,0x0);
			break;
		case 0x3:
			graphics_terminal_setrgb(0,0xE0,0xE0);
			break;
		case 0x4:
			graphics_terminal_setrgb(0xE0,0,0);
			break;
		case 0x5:
			graphics_terminal_setrgb(0xE0,0,0xE0);
			break;
		case 0x6:
			graphics_terminal_setrgb(0x30,0xE0,0xE0);
			break;
		case 0x7:
			graphics_terminal_setrgb(0xE0,0xE0,0xE0);
			break;
		case 0x8:
			graphics_terminal_setrgb(0xD0,0xE1,0xD0);
			break;
		case 0x9:
			graphics_terminal_setrgb(0x0,0x0,0xFF);
			break;
		case 0xA:
			graphics_terminal_setrgb(0,0xE0,0x0);
			break;
		case 0xB:
			graphics_terminal_setrgb(0,0xFF,0xFF);
			break;
		case 0xC:
			graphics_terminal_setrgb(0xFF,0x0,0x0);
			break;
		case 0xD:
			graphics_terminal_setrgb(0xFF,0,0xFF);
			break;
		case 0xE:
			graphics_terminal_setrgb(0xFF,0xFF,0x0);
			break;
		case 0xF:
			graphics_terminal_setrgb(0xFF,0xFF,0xFF);
		default:
			graphics_terminal_setrgb(0xFF,0xFF,0xFF);
			break;
	}
#endif
}

void text_console_reset_color() {
#ifdef GRAPHICS_DEVICE_PL100
	graphics_terminal_resetrgb();
#endif
}

void text_console_scroll(int x, int x2) {
}

void text_console_setcursor(int x, int y) {
}

void processor_halt()
{
	for(;;) {
		
	}
}

void vmm_shim_doBoardSetup() {
}