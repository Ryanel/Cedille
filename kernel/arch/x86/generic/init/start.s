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
    ; This is the kernels true "entry point"
    ; What we need to do first is setup the stack to a known state.
    mov esp, stack_top
    ; Okay, the stack is setup so we can call functions, and we have 8kb of stack to work with.
    ; Since kernel_entry is the root of the callframe (as there is no callframe yet!) we can just call it.
    call kernel_entry
    ; If the computer gets here, something wrong happened and kernel_entry aborted. 
    ; We know nothing about the state of the computer at this point, and can only halt in a loop after clearing interupts.
    cli
.error_loop: ; The . means its local, like static in C.
    hlt
    jmp .error_loop