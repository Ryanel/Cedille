#include <stddef.h>
#include <stdint.h>
#ifdef ARCHx86
#include <arch/x86/ports.h>
#endif
#include <text_console.h>
volatile uint8_t term_x = 0;
volatile uint8_t term_y = 0;

void scroll() {
    if (term_y >= 25) {
        text_console_scroll(0, 24);
        term_y = 24;
    }
}

void text_console_printc(char c) {
    if (c == 0x08 && term_x) {
        term_x--;
    } else if (c == 0x09) {
        term_x = (term_x+8) & ~(8-1);
    } else if (c == '\r') {
       term_x = 0;
    } else if (c == '\n') {
        #ifndef ARCHx86
        text_console_printchar(c,term_x, term_y);
        #endif
        term_x = 0;
        term_y++;
    } else if (c >= ' ') {
        text_console_printchar(c,term_x, term_y);
        term_x++;
    }
    if (term_x >= 80) {
        term_x = 0;
        term_y++;
    }
    // Scroll the screen if needed.
    scroll();
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
    #ifdef ARCHx86
        uint16_t offset;
        outb(0x3D4, 14);
        offset = inb(0x3D5) << 8;
        outb(0x3D4, 15);
        offset |= inb(0x3D5);
        term_x = offset % 80;
        term_y = offset / 80;
    #else
        term_x = 0;
        term_y = 0;
    #endif
    //video_setcursor(term_x, term_y);
}