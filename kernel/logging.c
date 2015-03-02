#include <stdio.h>
#include <string.h>
#include <cedille/logging.h>
#include <cedille/text_console.h>
#include <cedille/kconfig.h>

/**
Logs output to system console. Works as an enchanced printf.
@param[in] severity  The log level - determines color and visibility.
@param[in] type  The kind or type of the entry. The named unit
@param[in] fmt  The message format. Passed directly to printf.
@param[in] ...  Any paramaters to pass
**/
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
			text_console_change_color(0xD);
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