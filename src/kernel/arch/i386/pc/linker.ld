ENTRY(_start)

SECTIONS
{
	. = 0xC0100000;

    kernel_link_memory_start = .;

	.text ALIGN(0x1000) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(0x1000) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(0x1000) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(0x1000) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
        *(.bootstrap_stack)
	}
    
    /DISCARD/ :
    {
        *(.comment)
    }

    kernel_link_memory_end = .;
}
