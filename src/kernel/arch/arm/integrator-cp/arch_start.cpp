#include <kernel/arch/arm/integrator-cp/drivers/serial_log.h>
#include <kernel/log.h>
#include <kernel/version.h>
SerialLogger serialLog;

void init_logging() {
    g_log.Init(&serialLog);
    g_log.Log(LOG_DEBUG, "arm", "Initialised serial logging");
}

extern "C" void kernel_start(void) {
    init_logging();

    kernel_print_version();

    g_log.Log(LOG_DEBUG, "log", "Debug!");
    g_log.Log(LOG_INFO, "log", "Info!");
    g_log.Log(LOG_NOTICE, "log", "Notice!");
    g_log.Log(LOG_MESSAGE, "log", "Message!");
    g_log.Log(LOG_WARN, "log", "Warn!");
    g_log.Log(LOG_ERROR, "log", "Error :(");
    g_log.Log(LOG_PANIC, "log", "PANIC!!!");
}
