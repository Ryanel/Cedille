#include <../../build/git-info.h>
#include <logging.h>
#include <stdio.h>
void low_printname() {
    printf("\nThe Cedille Kernel (v.000.8-dev) [rev. %s]\n",GIT_SHORT_HASH);
    printf("Copyright (C) Corwin McKnight 2014-2015. Licensed under The MIT License.\n");
    #ifdef OPTION_ENABLE_BUILDENV_PRINTING
    printf("Source: %s\n",BUILD_SOURCE);
    printf("Architecture: %s/%s\n",ARCH_S,BOARD_S);
    #endif
    printf("\n");
}