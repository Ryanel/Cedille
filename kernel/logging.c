#include <logging.h>
void log(int type, char * s) {
    //Translates types into strings, then prints the result using log low.
    switch(type) {
        case LOG_STATECHANGE:
            log_low("[State] ");
            break;
        case LOG_KERNEL:
        default:
            log_low("[Kernel] ");
            break;
    }
    log_low(s);
}