#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cedille/ktypes.h>
#include <cedille/kconfig.h>
#include <cedille/kprocess.h>
#include <cedille/error.h>
#include <cedille/logging.h>

kprocess_t kprocess_list[CONFIG_KPROCESS_MAX];

void kprocess_doUpdateForced() {
	for (int i=0; i < CONFIG_KPROCESS_MAX; i++) {
		if (kprocess_list[i].update != NULL) {
			kprocess_list[i].update();
		}
	}
}

void kprocess_doUpdate(kernel_time_t localtime) {
	for (int i=0; i < CONFIG_KPROCESS_MAX; i++) {
		if(kprocess_list[i].updateTime <= localtime) {
			if (kprocess_list[i].update != NULL) {
				kprocess_list[i].update();
				if(kprocess_list[i].deltaTime == 0) { // Delete entry
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
	for(kprocess_next=0; kprocess_next <= CONFIG_KPROCESS_MAX; kprocess_next++) {
		if (kprocess_next == CONFIG_KPROCESS_MAX) {
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
}

void kprocess_init() {
	memset(&kprocess_list,0,sizeof(kprocess_t)*CONFIG_KPROCESS_MAX);
	printk(LOG_INFO,"kproc","Initialised kernel process list.\n");
}