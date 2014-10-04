#include <state.h>

int system_state = STATE_EARLY;

void sys_change_state(int state)
{
    system_state = state;
}

void idle() {
    for(;;){
        //Insert statement to turn off processor here.
    }
}