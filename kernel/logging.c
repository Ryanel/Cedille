#include <logging.h>
void log(int type, char * s) {
    //Translates types into strings, then prints the result using log low.
    log_low("[kernel]:");log_low(s);
}