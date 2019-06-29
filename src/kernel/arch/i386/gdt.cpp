#include <kernel/arch/i386/gdt.h>

extern "C" void gdt_flush(void* ptr);

void x86TableGDT::SetGate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
                          uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);

    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;

    gdt[num].access = access;
}

void x86TableGDT::Setup() {
    ptr.limit = (sizeof(struct gdt_entry) * numEntries) - 1;
    ptr.base = (uint32_t)&gdt;

    SetGate(0, 0, 0, 0, 0);                 // Null segment
    SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment
    SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User mode code segment
    SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User mode data segment
    //SetGate(5, (uint32_t)&tls_bss_middle, 0xFFFFFFFF, 0x93, 0xCF);  // GS TLS segment
    //SetGate(6, (uint32_t)&tls_bss_middle, 0xFFFFFFFF, 0x93, 0xCF);  // FS TLS segment
    gdt_flush((void*)&ptr);
}

x86TableGDT g_table_gdt;