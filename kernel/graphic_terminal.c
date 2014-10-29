#include <stdint.h>
#include <stddef.h>
#include <data/font-8x8.h>
void graphics_terminal_drawpixel(int x,int y, uint8_t r,uint8_t g,uint8_t b);

static const uint8_t graphics_terminal_default_r = 0xFF;
static const uint8_t graphics_terminal_default_g = 0xFF;
static const uint8_t graphics_terminal_default_b = 0xFF;

uint8_t graphics_terminal_r = 0xFF;
uint8_t graphics_terminal_g = 0xFF;
uint8_t graphics_terminal_b = 0xFF;

void graphics_terminal_setrgb(uint8_t r,uint8_t g,uint8_t b) {
	graphics_terminal_r = r;
	graphics_terminal_g = g;
	graphics_terminal_b = b;
}

void graphics_terminal_resetrgb() {
	graphics_terminal_r = graphics_terminal_default_r;
	graphics_terminal_g = graphics_terminal_default_g;
	graphics_terminal_b = graphics_terminal_default_b;
}

void graphics_terminal_drawchar(char c, int x, int y)
{
	uint8_t row = 0;
	uint32_t col = x * 8;
	uint32_t ry  = y * 9;
	for (row = 0; row < 8; row++) {
		
		for (uint8_t i = 0; i < 8; i++) {
			if (font8x8_basic[(uint8_t)c][row] & (1 << i)) 
			{
				graphics_terminal_drawpixel(col+i,ry+row, graphics_terminal_r,graphics_terminal_g,graphics_terminal_b);
			}
		}
	}
	
}