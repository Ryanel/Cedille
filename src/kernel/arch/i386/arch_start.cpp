#include <stdio.h>
#include <stdlib.h>

#include <kernel/arch/i386/drivers/bga.h>
#include <kernel/arch/i386/drivers/bga_log.h>
#include <kernel/arch/i386/drivers/serial_log.h>
#include <kernel/arch/i386/textmode_log.h>
#include <kernel/log.h>
#include <kernel/main.h>

#define USE_BGA
//#define USE_SERIAL
//#define USE_TEXTMODE

x86TextModeLog textmodeLog;
x86BGALog bgaLog;
SerialLog serialLog;

void init_logging() {
#ifdef USE_SERIAL
    g_log.Init(&serialLog);
#endif

#ifdef USE_TEXTMODE
    g_log.Init(&textmodeLog);
#endif

#ifdef USE_BGA
    g_log.Init(&textmodeLog);
    g_log.Log(LOG_INFO, "log", "Initialised 80x25 text mode...");
    // Check if we have BGA
    if (x86driver_bga.Supported()) {
        x86driver_bga.Initialise();

        g_log.Init(&bgaLog);

        g_log.Log(LOG_INFO, "log",
                  "Switched from Textmode to BGA Graphics Adapter");
    } else {
        g_log.Log(LOG_WARN, "log",
                  "BGA unsupported, continuing to use 80x25 text mode.");
    }
#endif
    g_log.Log(LOG_DEBUG, "log", "Log Initialised");
}

extern "C" void kernel_start(void) {
    init_logging();

    // Init GDT

    kmain();
}
