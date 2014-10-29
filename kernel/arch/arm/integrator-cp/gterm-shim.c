#include <arch/arm/arm4/integrator-cp/pl110.h>
void graphics_terminal_drawpixel(int x,int y, uint8_t r,uint8_t g,uint8_t b) {
	arm4_integrator_cp_pl110_write(x,y,r,g,b);
}