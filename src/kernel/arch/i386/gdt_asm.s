
.global gdt_flush
.intel_syntax noprefix

gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]

    jmp 0x08:.flush
.flush:
    ret
