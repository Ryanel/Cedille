; List references
extern kernel_entry
global _start
; Multiboot Header

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
    align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM

; Bootstrap Stack
section .bootstrap_stack
    align 4
        stack_bottom:
            times 8192 db 0
        stack_top:

_start:
	; Setup Stack
    mov esp, stack_top
	; Call kernel
    call kernel_entry
    cli
.error_loop: ; The . means its local, like static in C.
    hlt
    jmp .error_loop