#include <stdio.h>
#include <string.h>
void printk(const char * type, const char *fmt, ...)
{
	if(strlen(type) > 6)
	{
		char array[6];
		array[0] = type[0];
		array[1] = type[1];
		array[2] = type[2];
		array[3] = type[3];
		array[4] = type[4];
		array[5] = '#';
		array[6] = 0;
		printf("%-6s| ",array);
	}
	else if(strcmp(type,"ok") == 0)
	{
		video_settextfore(0xA);
		printf("%-6s| ",type);
		video_reset_attr();
	}
	else if(strcmp(type,"fail") == 0)
	{
		video_settextfore(0x4);
		printf("%-6s| ",type);
		video_reset_attr();
	}
	else if(strcmp(type,"info") == 0 || strcmp(type,"status") == 0)
	{
		video_settextfore(0xB);
		printf("%-6s| ",type);
		video_reset_attr();
	}
	else if(strcmp(type,"cpu") == 0)
	{
		video_settextfore(0xD);
		printf("%-6s| ",type);
		video_reset_attr();
	}
	else if(strcmp(type,"device") == 0)
	{
		video_settextfore(0x9);
		printf("%-6s| ",type);
		video_reset_attr();
	}
	else
	{
		printf("%-6s| ",type);
	}
	va_list args;
	va_start(args, fmt);
	vprintf(fmt,args);
	va_end(args);
}