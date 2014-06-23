ENTRY (_start)

SECTIONS
{
    . = 0;
    _kernel_start = .;
    .text : { *(.text*) *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON*) }
    _kernel_end = .;
}