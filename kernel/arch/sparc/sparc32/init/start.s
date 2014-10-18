.extern kernel_entry
.global _start
_start:
    call kernel_entry
        nop

halt:
    call halt
        nop
