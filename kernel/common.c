#include <../../build/git-info.h>
#include <logging.h>
void low_printname() {
    log_low("The Cedille Kernel (v.000.7-rw) [rev. ");log_low(GIT_SHORT_HASH); log_low("]\n");
    #ifdef DEBUG
    log_low(" => Source: "); log_low(BUILD_SOURCE); log_low("\n");
    log_low(" => Architecture: "); log_low(ARCH_S); log_low("/"); log_low(BOARD_S); log_low("\n");
    log_low(" => Build Flags:{"); log_low( CCOMPILER_OPTIONS_S ); log_low("}\n");
    #endif
}