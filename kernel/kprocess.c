#include <logging.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ktypes.h>
#include <error.h>
#define KPROCESS_MAX 10
typedef struct {
	void (*update)(void);
	kernel_time_t updateTime;
	kernel_time_t deltaTime;
	char * name;
} kprocess_t;

kprocess_t kprocess_list[KPROCESS_MAX];

void kprocess_doUpdateForced() {
	for (int i=0; i < KPROCESS_MAX; i++) {
		if (kprocess_list[i].update != NULL) {
			kprocess_list[i].update();
		}
	}
}

void kprocess_doUpdate(kernel_time_t localtime) {
	for (int i=0; i < KPROCESS_MAX; i++) {
		if(kprocess_list[i].updateTime <= localtime) {
			if (kprocess_list[i].update != NULL) {
				kprocess_list[i].update();
				if(kprocess_list[i].deltaTime == 0) { // Delete entry
					printk(LOG_INTERNALS,"kproc","Deleting kprocess %s!\n",kprocess_list[i].name);
					kprocess_list[i].updateTime = 0;
					kprocess_list[i].update = 0;
					kprocess_list[i].name = NULL;
				}
				kprocess_list[i].updateTime += kprocess_list[i].deltaTime ;
				if((kprocess_list[i].updateTime + kprocess_list[i].updateTime) >= 1000) {
					kprocess_list[i].updateTime -= 1000;
				}
			}
		}
	}
}

void kprocess_create(char * name,kernel_time_t localtime,kernel_time_t deltatime, void (*callback)(void)) {
	int kprocess_next;
	for(kprocess_next=0; kprocess_next <= KPROCESS_MAX; kprocess_next++) {
		if (kprocess_next == KPROCESS_MAX) {
			panic("Failed to create kernel process; ran out of slots!\n");
		}
		if (kprocess_list[kprocess_next].update == NULL) {
			break;
		}
		
	}
	kprocess_list[kprocess_next].updateTime = localtime;
	kprocess_list[kprocess_next].deltaTime = deltatime;
	kprocess_list[kprocess_next].update = callback;
	kprocess_list[kprocess_next].name = name;
	printk(LOG_INTERNALS,"kproc","Created kprocess %s\n",name);
}

void kprocess_init() {
	memset(&kprocess_list,0,sizeof(kprocess_t)*KPROCESS_MAX);
	printk(LOG_INFO,"kproc","Initialised Kernel process list\n");
}