#ifndef LOGGING_H
#define LOGGING_H

#include <stddef.h>
#include <stdint.h>

#define DO_DEBUG_LOGGING 1

#define LOG_COMPLETE	1
#define LOG_FAIL		2
#define LOG_WARN		3
#define LOG_INFO		4
#define LOG_DEBUG		5
#define LOG_INTERNALS	6
#define LOG_UNKN		0
// Low level
void log_low(const char * str);
void low_printname();
void text_console_init();
// High level
void printk(const int severity,const char * type, const char *fmt, ...);
void logging_printbestunit(uint32_t bytes, uint8_t newline);

#endif
