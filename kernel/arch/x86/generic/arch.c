void text_console_print(const char *c);
void log_low(const char * str) {
    text_console_print(str);
}

void processor_halt() {
	asm("cli");
	for(;;) {
		asm("hlt");
	}
	
}