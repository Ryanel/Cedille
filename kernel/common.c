#include <../../build/git-info.h>
#include <logging.h>
void low_printname() {
    log_low("The Cedille Kernel {v.000.7-rw} [rev. ");log_low(GIT_SHORT_HASH); log_low("]\n");
}