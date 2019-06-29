#include <stdio.h>
#include <stdlib.h>

#include <kernel/arch/i386/drivers/bga.h>
#include <kernel/arch/i386/drivers/bga_log.h>
#include <kernel/arch/i386/drivers/serial_log.h>
#include <kernel/arch/i386/drivers/textmode_log.h>
#include <kernel/arch/i386/gdt.h>
#include <kernel/boot/multiboot.h>
#include <kernel/log.h>
#include <kernel/main.h>
#include <kernel/panic.h>
#include <kernel/version.h>

extern uint32_t kernel_link_memory_start;
extern uint32_t kernel_link_memory_end;

//#define USE_BGA
//#define USE_SERIAL
#define USE_TEXTMODE

x86TextModeLog textmodeLog;
x86BGALog bgaLog;
SerialLog serialLog;

void init_logging() {
#ifdef USE_SERIAL
    g_log.Init(&serialLog);
#endif

#ifdef USE_TEXTMODE
    g_log.Init(&textmodeLog);
    g_log.Log(LOG_INFO, "log", "Initialised text mode...");
#endif

#ifdef USE_BGA
    g_log.Init(&textmodeLog);
    g_log.Log(LOG_INFO, "log", "Initialised text mode...");
    // Check if we have BGA
    if (x86driver_bga.Supported()) {
        x86driver_bga.Initialise();

        g_log.Init(&bgaLog);

        g_log.Log(LOG_INFO, "log",
                  "Switched from Textmode to BGA Graphics Adapter");
    } else {
        g_log.Log(LOG_WARN, "log",
                  "BGA unsupported, continuing to use text mode.");
    }
#endif
    g_log.Log(LOG_DEBUG, "log", "Log Initialised");
}

void load_memory_map(multiboot_info_t* mbd) {
    const char* typestrings[] = {
        "Used",
        "Avalable",
        "ACPI",
        "NVS",
        "Bad RAM",
        "Unknown"};

    unsigned int entries = 0;
    uint32_t avalable_memory_estimate = 0;
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbd->mmap_addr;

    g_log.Log(LOG_DEBUG, "mm", "Memory map address is at: 0x%X and is 0x%X bytes long", mbd->mmap_addr, mbd->mmap_length);

    // Loop over the memory map.
    while ((multiboot_uint32_t)mmap < mbd->mmap_addr + mbd->mmap_length) {
        // Get the next entry address
        mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));

        // Sanitize the type to prevent printing arbitrary strings. Probably not needed.
        uint32_t type_sanitized = mmap->type;
        if (type_sanitized > sizeof(typestrings)) {
            type_sanitized = sizeof(typestrings);
        }

        int log_type = LOG_DEBUG;

        if (type_sanitized == 1) {
            avalable_memory_estimate += mmap->len_lo;
            log_type = LOG_MESSAGE;
        }

        // Print debug info to console
        g_log.Log(log_type, "mm", "%8s 0x%08X => 0x%08X", typestrings[type_sanitized], mmap->addr_lo, (mmap->addr_lo + mmap->len_lo));

        entries++;
    }

    uint32_t avalable_memory_est_mb = avalable_memory_estimate / 1024 / 1024;

    g_log.Log(LOG_INFO, "mm", "Memory map has %d entries", entries);
    g_log.Log(LOG_NOTICE, "mm", "Have about 0x%08X bytes (%d MB) free", avalable_memory_estimate, avalable_memory_est_mb);
    g_log.Log(LOG_DEBUG, "mm", "Kernel is at 0x%08X => 0x%08x", &kernel_link_memory_start, &kernel_link_memory_end);
}

extern "C" void kernel_start(multiboot_info_t* mbd, unsigned int magic) {
    return;
    init_logging();

    // Verify boot state...
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        g_log.Log(LOG_PANIC, "multiboot", "Multiboot magic was: 0x%X", magic);
        panic("Multiboot magic was incorrect. Should be 0x2BADB002");
    }
    // Print version first thing.
    kernel_print_version();

    // Init GDT
    g_log.Log(LOG_DEBUG, "start", "Initialising GDT");
    g_table_gdt.Setup();
    g_log.Log(LOG_DEBUG, "start", "Initialised GDT successfully.");

    // Now lets poke around and see what memory we can access
    load_memory_map(mbd);

    g_log.Log(LOG_NOTICE, "start", "Finished early setup, entering kernel main!");
    kmain();
}
