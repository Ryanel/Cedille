#include <stdio.h>
#include <stdlib.h>

#include <kernel/main.h>

extern "C" void kernel_start(void)
{
    kmain();
}
