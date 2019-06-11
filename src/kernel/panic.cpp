#include <kernel/control.h>
#include <kernel/log.h>

const char *panicstr;

extern "C" [[noreturn]] void panic(const char *s) {
    // Save string if no terminal access...
    panicstr = s;

    // Log error to console
    g_log.Log(LOG_PANIC, "panic", s);

    // Halt the system:
    kernel_halt_system();
}