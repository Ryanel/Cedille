#ifndef KPROCESS_H
#define KPROCESS_H
#include <cedille/ktypes.h>

typedef struct {
	void (*update)(void);
	kernel_time_t updateTime;
	kernel_time_t deltaTime;
	char * name;
} kprocess_t;

void kprocess_doUpdateForced();
void kprocess_doUpdate(kernel_time_t localtime);
void kprocess_create(char * name,kernel_time_t localtime,kernel_time_t deltatime, void (*callback)(void));
void kprocess_init();
#endif