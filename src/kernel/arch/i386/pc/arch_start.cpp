#include <stdio.h>
#include <stdlib.h>

#include <kernel/arch/i386/gdt.h>
#include <kernel/boot/multiboot.h>
#include <kernel/log.h>
#include <kernel/main.h>
#include <kernel/panic.h>
#include <kernel/types.h>
#include <kernel/version.h>

void load_memory_map(multiboot_info_t* mbd);
void init_logging();

extern "C" void kernel_start(multiboot_info_t* mbd, unsigned int magic) {
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
