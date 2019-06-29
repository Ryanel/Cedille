#include <kernel/log.h>
#include <kernel/version.h>

void kernel_print_version() {
    g_log.Log(LOG_INFO, "kernel", "Cedille v. 0.0.0.1");
    g_log.Log(LOG_INFO, "kernel", "Written by Corwin McKnight");
    g_log.Log(LOG_INFO, "kernel", "Running on: %s %s", ARCH_NAME, PLATFORM_NAME);
}