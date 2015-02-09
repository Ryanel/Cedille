OUTPUT_FORMAT(binary)
ENTRY(_start)
 
SECTIONS
{
    . = 0x3980;
    .main : 
    {
        LONG(0x01030107);
        LONG(_kernel_end - _kernel_start);
        LONG(0);
        LONG(0);
        LONG(0);
        LONG(0);
        LONG(0);
        LONG(0);
        _kernel_start = .;
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
        _kernel_end = .;
    }
}