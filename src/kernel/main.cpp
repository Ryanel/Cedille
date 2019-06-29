#include <kernel/log.h>
#include <kernel/panic.h>

int kmain() {
    g_log.Log(LOG_NOTICE, "kernel", "Entered Kernel Main.");
    while (true) {
    }
    panic("Kernel main exited...");
    return 0;
}