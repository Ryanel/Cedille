#ifndef STATE_H
#define STATE_H
#define STATE_EARLY 1
#define STATE_SERVICE_INIT 2
#define STATE_HALTED 0x1000
void idle();
void sys_change_state(int state);
#endif