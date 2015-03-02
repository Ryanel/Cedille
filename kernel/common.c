#include <../../temp/git-info.h>
#include <stdio.h>
#include <cedille/text_console.h>
#include <cedille/kprocess.h>
#include <cedille/kconfig.h>
void low_printname() {
    printf("\nThe Cedille Kernel (v.%s-%s) [build %s]\n",CONFIG_KERNEL_VERSION_S,CONFIG_KERNEL_TYPE_S,GIT_SHORT_HASH);
    printf("Copyright (C) Corwin McKnight 2014-2015. Licensed under The MIT License.\n");
    #ifdef OPTION_ENABLE_BUILDENV_PRINTING
    printf("Source: %s\n",BUILD_SOURCE);
    printf("Architecture: %s/%s\n",ARCH_S,BOARD_S);
    #endif
    printf("\n");
}

/**
Does periodic tasks
force: Forces all tasks to be run.
tick: Used in timing functions.
**/
//TODO: Move to scheduler code
void kernel_doperiodic(int force, int tick) {
	if(force) {
		kprocess_doUpdateForced();
		text_console_fb_flush(); //This gets called as a kernel process, but its okay to do it as no data should have been printed to screen.
	}
}