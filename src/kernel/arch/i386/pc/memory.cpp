#include <kernel/boot/multiboot.h>
#include <kernel/log.h>
#include <kernel/panic.h>
#include <kernel/types.h>
#include <string.h>

#define KERNEL_PHYS_TO_VIRTUAL(x) x + 0xC0000000
#define MAX_MEMORY_REGION_ENTRIES 32
extern virtual_address_t kernel_link_memory_start;
extern virtual_address_t kernel_link_memory_end;

virtual_address_t kPhysToVirtual(phys_address_t x) {
    return x + 0xC0000000;
}

phys_address_t kVirtualToPhys(virtual_address_t x) {
    return x - 0xC0000000;
}

struct MemoryRegion {
    enum Type {
        Used = 0,
        Avalable = 1,
        ACPI = 2,
        NVS = 3,
        Other,
        ISAHole
    };
    Type type;
    uintptr_t start_address;
    uintptr_t size;
    bool valid = false;

    uintptr_t end_address() {
        return start_address + size - 1;
    }
};

MemoryRegion boot_memory_table[MAX_MEMORY_REGION_ENTRIES];

void init_table() {
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        boot_memory_table[i].valid = false;
    }
}
static const char* typestrings[] = {
    "Used",
    "Avalable",
    "ACPI",
    "NVS",
    "Bad RAM",
    "ISA",
    "Unknown"};

void display_memory_table(MemoryRegion table[]) {
    // Display old memory map before cleanup
    g_log.Log(LOG_DEBUG, "mmap", "IX |  Type  |   Start   |      End");
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        if (table[i].valid) {
            g_log.Log(LOG_DEBUG, "mmap", "%2d  %8s 0x%08X => 0x%08X", i, typestrings[table[i].type], table[i].start_address, table[i].end_address());
        }
    }
}

void cleanup_mappings() {
    display_memory_table(boot_memory_table);
    g_log.Log(LOG_INFO, "mmap", "Cleaning up memory map from GRUB...");
    MemoryRegion new_memory_table[MAX_MEMORY_REGION_ENTRIES];
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        new_memory_table[i].valid = false;
    }
    int new_entries = 0;

    // First, copy table, and fix any nonsense stuff
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        if (boot_memory_table[i].valid) {
            if (boot_memory_table[i].size == 0) {
                continue;
            }

            new_memory_table[new_entries] = boot_memory_table[i];
            new_entries++;
        }
    }

    // Secondly, see if any avalable block is intersected by any other kind of block.
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        bool found_collision = false;
        if (new_memory_table[i].valid) {
            if (new_memory_table[i].type != MemoryRegion::Type::Avalable) {
                continue;
            }
            //g_log.Log(LOG_INFO, "memcln", "Found avalable region %d. Checking for intersections", i);
            MemoryRegion avalable_region = new_memory_table[i];
            for (int k = 0; k < MAX_MEMORY_REGION_ENTRIES; k++) {
                if (new_memory_table[k].valid) {
                    if (new_memory_table[k].type == MemoryRegion::Type::Avalable) {
                        continue;
                    }
                    // Here we have a non-avalable region. Check to see if it intersects with the avalable block.
                    MemoryRegion suspect = new_memory_table[k];

                    bool inside_test = (suspect.start_address > avalable_region.start_address) &&
                                       (suspect.end_address() < avalable_region.end_address());
                    bool start_test = (suspect.start_address < avalable_region.start_address) &&
                                      (suspect.end_address() < avalable_region.end_address()) &&
                                      (suspect.end_address() > avalable_region.start_address);
                    bool end_test = suspect.start_address > avalable_region.start_address &&
                                    suspect.start_address < avalable_region.end_address();

                    if (inside_test) {
                        found_collision = true;
                        g_log.Log(LOG_DEBUG, "memcln", "Region %d (type %s) is inside %d!", k, typestrings[new_memory_table[k].type], i);

                        // We must split the avalable block into two blocks...
                        // Create second block AFTER the collision =====XXXX[this]
                        MemoryRegion avalable_region_new = avalable_region;
                        avalable_region_new.start_address = suspect.end_address() + 1;
                        avalable_region_new.size = avalable_region.size - suspect.size - (suspect.start_address - avalable_region.start_address);
                        new_memory_table[new_entries] = avalable_region_new;
                        new_entries++;
                        // Truncate before the collision [this]XXXX======
                        avalable_region.size = suspect.start_address - avalable_region.start_address;
                        new_memory_table[i] = avalable_region;

                        break;
                    }
                    if (start_test) {
                        g_log.Log(LOG_WARN, "memcln", "Region %d cuts off the start of %d!", k, i);

                        break;
                    }
                    if (end_test) {
                        g_log.Log(LOG_WARN, "memcln", "Region %d cutting off the end of %d!", k, i);
                        break;
                    }
                }
            }

            if (found_collision) {
                //display_memory_table(new_memory_table);
                g_log.Log(LOG_DEBUG, "memcln", "Region %d collided, checking for more collisions...", i);
                i--;
                //asm("hlt");
            }
        }
    }

    // Arrange in ascending order
    for (int i = 0; i < new_entries; ++i) {
        for (int j = i + 1; j < new_entries; ++j) {
            if (new_memory_table[i].start_address > new_memory_table[j].start_address) {
                MemoryRegion a = new_memory_table[i];
                new_memory_table[i] = new_memory_table[j];
                new_memory_table[j] = a;
            }
        }
    }

    // Finally, copy back
    new_entries = 0;
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        boot_memory_table[i].valid = false;
    }
    for (int i = 0; i < MAX_MEMORY_REGION_ENTRIES; i++) {
        if (new_memory_table[i].valid) {
            boot_memory_table[new_entries] = new_memory_table[i];
            new_entries++;
        }
    }

    display_memory_table(boot_memory_table);
}

void load_memory_map(multiboot_info_t* mbd) {
    init_table();
    unsigned int entries = 0;
    uintptr_t avalable_memory_estimate = 0;
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(mbd->mmap_addr + 0xC0000000);

    g_log.Log(LOG_DEBUG, "mmap", "Memory map address is @ 0x%X and is 0x%X bytes long", KERNEL_PHYS_TO_VIRTUAL(mbd->mmap_addr), mbd->mmap_length);

    // Loop over the memory map.
    while ((multiboot_uint32_t)mmap < KERNEL_PHYS_TO_VIRTUAL(mbd->mmap_addr) + mbd->mmap_length) {
        // Get the next entry address
        mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));

        // Sanitize the type to prevent printing arbitrary strings. Probably not needed.

        if (mmap->type == 1) {
            avalable_memory_estimate += mmap->len_lo;
        }

        // Now add this as a memory region.
        boot_memory_table[entries].type = (MemoryRegion::Type)mmap->type;
        boot_memory_table[entries].start_address = mmap->addr_lo;
        boot_memory_table[entries].size = mmap->len_lo;
        boot_memory_table[entries].valid = true;
        entries++;
    }

    uintptr_t avalable_memory_est_mb = avalable_memory_estimate / 1024 / 1024;

    g_log.Log(LOG_DEBUG, "mmap", "Memory map has %d entries", entries);
    g_log.Log(LOG_DEBUG, "mmap", "Have about 0x%08X bytes (%d MB) free", avalable_memory_estimate, avalable_memory_est_mb);
    g_log.Log(LOG_DEBUG, "mmap", "Kernel is at 0x%08X => 0x%08X", &kernel_link_memory_start, &kernel_link_memory_end);

    if (avalable_memory_estimate < 1024 * 1024 * 16) {  // 4MB
        panic(
            "*** LOW MEMORY *** \n"
            "System has less then 16MB of RAM free.\n"
            "Cedille requires a minimum of 16MB of RAM to boot.");
    }
    cleanup_mappings();
}