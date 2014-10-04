#ifndef STATE_H
#define STATE_H
#define STATE_ERRORED 0
#define STATE_EARLY 1
#define STATE_HARDWARE_INIT 2
#define STATE_SERVICE_INIT 3
#define STATE_HANDING 4
#define STATE_IDLE 5
#define STATE_UNKN 6
void idle();
void sys_change_state(int state);
#endif