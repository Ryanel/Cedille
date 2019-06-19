.section .text
.global kernel_halt_system
.type kernel_halt_system, @function
kernel_halt_system:
    cli
.continue_halt:
    hlt
    jmp .continue_halt
