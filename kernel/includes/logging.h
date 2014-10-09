#ifndef LOGGING_H
#define LOGGING_H
void log_low(const char * str);
void log(int type, char * s);
void low_printname();
void text_console_init();
#define LOG_KERNEL 0x0
#define LOG_INFO 0x1
#define LOG_DEBUG 0x2
#define LOG_STATECHANGE 0x3
#define LOG_HARDWARE 0x4
#define LOG_CONFIGURATION 0x4
#endif