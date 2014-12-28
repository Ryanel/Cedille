ENTRY(bootstrap)
KERNEL_VMA = 0xC0000000;
KERNEL_LMA = 1M;
SECTIONS
{
	. = KERNEL_LMA;
	
    .bootstrap :
    {
        kernel/arch/x86/amd64/init/bootstrap.o (.text)
    }

    . += KERNEL_VMA;
	
	_kernel_start = .;
    .text : AT(ADDR(.text) - KERNEL_VMA)
    {
        _code = .;
		*(EXCLUDE_FILE(*kernel/arch/x86/amd64/init/bootstrap.o) .text)
        *(.rodata*)
        . = ALIGN(4096);
    }

   .data : AT(ADDR(.data) - KERNEL_VMA)
   {
        _data = .;
        *(.data)
        . = ALIGN(4096);
   }

   .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA)
   {
       _ehframe = .;
       *(.eh_frame)
        . = ALIGN(4096);
   }

   .bss : AT(ADDR(.bss) - KERNEL_VMA)
   {
       _bss = .;
       *(.bss)

       /*
        * You usually need to include generated COMMON symbols
        * under kernel BSS section or use gcc's -fno-common
        */

        *(COMMON)
       . = ALIGN(4096);
   }

   _kernel_end = .;

   /DISCARD/ :
   {
        *(.comment)
   }
}