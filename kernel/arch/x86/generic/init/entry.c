#include <stddef.h>
#include <state.h>
#include <logging.h>

int kernel_entry (void) {

    text_console_init();
    low_printname();
    log(LOG_STATECHANGE,"Entering Boot Phase\n");
    log(LOG_HARDWARE,"Initialising Hardware\n");
    idle();
    return 0;
}