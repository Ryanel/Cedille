#include <arch/arm/arm4/integrator-cp/pl110.h>
#include <stdint.h>
#include <stddef.h>
uint16_t * volatile arm4_integrator_cp_pl_fb;
pl110_mmio_t	*plio;

void arm4_integrator_cp_pl110_write_index(uint32_t index,uint8_t r,uint8_t g,uint8_t b)
{
	arm4_integrator_cp_pl_fb[index] = b << (5 + 6) | g << 5 | r ;
}

void arm4_integrator_cp_pl110_write(int x, int y,uint8_t r,uint8_t g,uint8_t b)
{
	uint32_t place = (y*640)+x;
	arm4_integrator_cp_pl110_write_index(place,r,g,b);
}

void arm4_integrator_cp_pl110_clear()
{
	for (int x = 0; x < (640 * 480) - 10; ++x)
		arm4_integrator_cp_pl110_write_index(x,0,0,0);
}
void arm4_integrator_cp_pl110_test()
{

}

void arm4_integrator_cp_pl110_start(void)
{
	plio = (pl110_mmio_t*)PL110_IOBASE;
	plio->tim0 = 0x3f1f3f9c;
	plio->tim1 = 0x080b61df;
	plio->upbase = 0x200000;
	plio->control = 0x1829;
	arm4_integrator_cp_pl_fb = (uint16_t*)0x200000;
	arm4_integrator_cp_pl110_clear();
}