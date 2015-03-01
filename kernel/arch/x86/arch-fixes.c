#include <cedille/text_console.h>

void log_low(const char * str) {
    text_console_print(str);
}

void processor_halt() {
	asm("cli");
	for(;;) {
		asm("hlt");
	}
	
}

void graphics_terminal_drawpixel() {
}
//TEMP: IRQ1 will not complain about being unassigned
void irq1fix()
{
}