
#include <kernel/arch/i386/drivers/textmode_log.h>
#include <kernel/log.h>
//#define USE_BGA
//#define USE_SERIAL
#define USE_TEXTMODE

x86TextModeLog textmodeLog;
DummyLog dummyLog;
#ifdef USE_BGA
#include <kernel/arch/i386/drivers/bga.h>
#include <kernel/arch/i386/drivers/bga_log.h>

x86BGALog bgaLog;

static void init_bga() {
    g_log.Init(&textmodeLog);
    g_log.Log(LOG_INFO, "log", "Initialised text mode...");
    // Check if we have BGA
    if (x86driver_bga.Supported()) {
        x86driver_bga.Initialise();
        g_log.Init(&bgaLog);

    } else {
        g_log.Log(LOG_WARN, "log",
                  "BGA unsupported, continuing to use text mode.");
    }
}
#endif

#ifdef USE_TEXTMODE
static void init_textmode() {
    g_log.Init(&textmodeLog);
    g_log.Log(LOG_INFO, "log", "Initialised text mode...");
}
#endif

void init_logging() {
    g_log.Init(&dummyLog);
#ifdef USE_TEXTMODE
    init_textmode();
#endif
#ifdef USE_BGA
    init_bga();
#endif
    g_log.Log(LOG_DEBUG, "log", "Log Initialised");
    g_log.renderer.SignalFullRender();
}
