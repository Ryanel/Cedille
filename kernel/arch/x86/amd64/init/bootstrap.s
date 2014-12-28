.section .text
.code32
.global bootstrap
multiboot_header:
	.set ALIGN,    1<<0             # align loaded modules on page boundaries
	.set MEMINFO,  1<<1             # provide memory map
	.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
	.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
	.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
.intel_syntax noprefix

bootstrap:
	cld
	cli
	
halt32:	
	hlt
