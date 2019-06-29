#pragma once

#include <stdint.h>

class x86TableGDT {
   private:
    /// An entry in the GDT
    struct gdt_entry {
        unsigned short limit_low;
        unsigned short base_low;
        unsigned char base_middle;
        unsigned char access;
        unsigned char granularity;
        unsigned char base_high;
    } __attribute__((packed));

    /// Pointer to the GDT
    struct gdt_ptr {
        unsigned short limit;
        unsigned int base;
    } __attribute__((packed));

   private:
    /// The number of entries in the GDT
    const static int numEntries = 5;
    /// The actual GDT. Actually will contain the entries
    struct gdt_entry gdt[numEntries];
    struct gdt_ptr ptr;
    /// Sets the GDT entry in gdt.
    void
    SetGate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
            uint8_t gran);

   public:
    /// Sets up the GDT.
    void Setup();
};

extern x86TableGDT g_table_gdt;