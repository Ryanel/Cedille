#ifndef TEXT_CONSOLE_H
#define TEXT_CONSOLE_H

void text_console_printc(char c);
void text_console_print(const char *c);
void text_console_init();

//Needed to be implemented by a driver, unless impossible
void text_console_printchar(char c, uint8_t x, uint8_t y);
void text_console_setcursor(uint8_t x,uint8_t y);
void text_console_scroll();

void text_console_change_color(uint8_t color);
void text_console_reset_color();


void text_console_fb_addchar(char c, uint8_t x, uint8_t y);
void text_console_fb_shim_flush(uint8_t *flush,uint32_t scroll_y);
#endif