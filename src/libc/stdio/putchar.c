#include <stdio.h>

#if defined(__is_libk)

void kernel_log_char(char c);

#endif

int putchar(int ic)
{
#if defined(__is_libk)
    kernel_log_char((char)ic);
#else
    // TODO: Implement stdio and the write system call.
#endif
    return ic;
}
