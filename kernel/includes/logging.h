#ifndef LOGGING_H
#define LOGGING_H
void log_low(const char * str);
void low_printname();
void text_console_init();

#include <stddef.h>
#include <stdint.h>
int printk(const char *title, const char *fmt, ...);
void logging_printbestunit(uint32_t bytes, uint8_t newline);

#endif