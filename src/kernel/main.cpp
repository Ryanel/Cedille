#include <kernel/log.h>
#include <kernel/panic.h>

int kmain() {
    panic("Kernel main exited...");
    return 0;
}