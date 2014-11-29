#ifndef LOGGING_H
#define LOGGING_H

#include <stddef.h>
#include <stdint.h>

#define DO_DEBUG_LOGGING 1

// Low level
void log_low(const char * str);
void low_printname();
void text_console_init();
// High level
int printk(const char *title, const char *fmt, ...);
void logging_printbestunit(uint32_t bytes, uint8_t newline);

#endif