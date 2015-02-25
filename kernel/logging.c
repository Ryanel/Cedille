#include <stdio.h>
#include <string.h>
#include <logging.h>
/**
Logs output to system console. Works as an enchanced printf.
@param[in] type  The kind or type of the entry. Is autocolored if it is common.
@param[in] fmt  The message format. Passed directly to printf.
@param[in] ...  Any paramaters to pass
**/

#ifndef DEBUG
	#define DEBUG_ONLY_LOG return;
#else
	#define DEBUG_ONLY_LOG
#endif

void text_console_change_color(uint8_t color);
void text_console_reset_color();

void printk(const int severity,const char * type, const char *fmt, ...) {
	switch(severity) {
		case LOG_COMPLETE:
			text_console_change_color(0xA);
			break;
		case LOG_FAIL:
			text_console_change_color(0x4);
			break;
		case LOG_WARN:
			text_console_change_color(0xE);
			break;
		case LOG_INFO:
			text_console_change_color(0xB);
			break;
		case LOG_DEBUG:
			text_console_change_color(0xD);
			break;
		case LOG_INTERNALS:
			text_console_change_color(0x1E);
			break;
		case LOG_UNKN:
		default: // LOG_UNKN
			text_console_change_color(0x8);
			break;
	}
	printf("%s: ",type);
	text_console_reset_color();
	va_list args;
	va_start(args, fmt);
	vprintf(fmt,args);
	va_end(args);

}

/**
 Logs a version of bytes that is best for human readablity. Does not print a newline 
 @param[in] bytes How many bytes are there to judge?
 **/
void logging_printbestunit(uint32_t bytes, uint8_t newline)
{
	uint8_t unit = 0; // 0 bytes, 1 kib, 2 mib, 3 gib
	if(bytes >= 1024)
	{
		unit = 1;
	}
	if(bytes >= 1048576)
	{
		unit = 2;
	}
	if(bytes >= 1073741824)
	{
		unit = 3;
	}
	switch (unit) {
		case 0:
			printf("%d %s",bytes,"b");
			break;
		case 1:
			printf("%d %s",bytes / 1024,"KiB");
			break;
		case 2:
			printf("%d %s",(bytes / 1024) / 1024,"MiB");
			break;
		case 3:
			printf("%d %s",(bytes / 1024) / 1024,"MiB");
			break;
	}
	if(newline)
	{
		printf("\n");
	}
}
