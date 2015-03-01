#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <cedille/ktypes.h>
#include <cedille/text_console.h>
#include <cedille/kprocess.h>
#ifdef ARCHx86
#include <arch/x86/ports.h>
#endif
//TODO: Move this and other configuration variables to a kconfig.h
#define TERM_FB_MX 80
#define TERM_FB_MY 25
#define TERM_FB_SCREENS 4

volatile uint8_t term_x = 0;
volatile uint8_t term_y = 0;
uint32_t scroll_y = 0;
int term_fb_flag_modified = 0;
#ifdef ARCHx86
uint8_t text_console_fb[TERM_FB_MX * TERM_FB_MY * TERM_FB_SCREENS * 2]; // Stores attribute byte as well on x86.
#else
uint8_t text_console_fb[TERM_FB_MX * TERM_FB_MY * TERM_FB_SCREENS]; 
#endif

void scroll() {
	if (term_y >= 25) {
		text_console_scroll(0, 24);
		term_y = 24;
	}
}

void text_console_printc(char c) {
	switch(c) {
		case 0x08:
			if(term_x) {
				term_x--;
			}
			break;
		case 0x09:
			term_x = (term_x+8) & ~(8-1);
			break;
		case '\r':
			term_x = 0;
			break;
		case '\n':
			#ifndef ARCHx86
			#ifndef ARCHarm
			text_console_fb_addchar(c,term_x,term_y);
			#endif
			text_console_printchar(c,term_x, term_y);	
			#else
			term_fb_flag_modified = 1;
			#endif
			term_x = 0;
			term_y++;
			break;
		default:
			#ifndef ARCHarm
			text_console_fb_addchar(c,term_x,term_y);
			#else
			text_console_printchar(c,term_x, term_y);
			#endif	
			term_x++;
			break;		
	}
	
	if (term_x >= TERM_FB_MX) {
		term_x = 0;
		term_y++;
	}
	// Scroll the screen if needed.
	text_console_scroll();
	// Move the hardware cursor.
	text_console_setcursor(term_x, term_y);
}

void text_console_print(const char *c) {
	int i = 0;
	while (c[i]) {
		text_console_printc(c[i++]);
	}
}



void text_console_init() {
#ifndef ARCHarm
	kprocess_create("kfbrefresh",50,100,&text_console_fb_flush);
#endif
}

#ifdef ARCHx86
uint8_t text_console_fb_shim_x86_addattribute();
#endif
void text_console_fb_addchar(char c, uint8_t x, uint8_t y) {
	#ifdef ARCHx86
	text_console_fb[((y * TERM_FB_MX) + x) * 2] = c; // Multiply by 2 so you can add attribute byte.
	text_console_fb[(((y * TERM_FB_MX) + x) * 2) + 1] = text_console_fb_shim_x86_addattribute();
	#else
	text_console_fb[(y * TERM_FB_MX) + x] = c; 
	#endif
	term_fb_flag_modified = 1;
}

void text_console_fb_flush() {
	if(term_fb_flag_modified == 1) {
		text_console_fb_shim_flush((uint8_t*)&text_console_fb,scroll_y);
		term_fb_flag_modified = 0;
	}
}

void text_console_scroll() {
	if(TERM_FB_MY + scroll_y < term_y){
		scroll_y++;
	}
	// Resets if it gets to big
	// TODO: Replace this with a less destructive thingy
	if(scroll_y >= (TERM_FB_MY - 2) * TERM_FB_SCREENS) {
		uint32_t screensize = (TERM_FB_MX * TERM_FB_MY);
		#ifdef ARCHx86
		screensize = screensize * 2;
		#endif
		memset(&text_console_fb,0,screensize * TERM_FB_SCREENS);
		scroll_y = 0;
		term_y = 0;
	}
}