#include <../../build/git-info.h>
#include <logging.h>
#include <stdio.h>
void low_printname() {
    printf("The Cedille Kernel (v.000.7-rw) [rev. %s]\n",GIT_SHORT_HASH);
    #ifdef DEBUG
    printf(" => Source: %s\n",BUILD_SOURCE);
    printf(" => Architecture: %s/%s\n",ARCH_S,BOARD_S);
    printf(" => Build Flags:{%s}\n",CCOMPILER_OPTIONS_S);
    #endif
}