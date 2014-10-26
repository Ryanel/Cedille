#include <state.h>
#include <stdint.h>
uint32_t system_state = STATE_EARLY;

void sys_change_state(int state)
{
    system_state = state;
}

void idle() {
	system_state = 0;
    for(;;){
    }
}