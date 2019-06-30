# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
    # Setup paging
    
    # edi = physical address of boot_page_table_1
    movl $(boot_page_table1 - 0xC0000000), %edi

    # Map pages 0 -> 1023
    movl $0, %esi
    movl $1023, %ecx
1:
	# Map 0x00000000 -> kernel_memory_end
	cmpl $(0), %esi # Start at 0x00000000
	jl 2f
	cmpl $(kernel_link_memory_end - 0xC0000000), %esi
	jge 3f

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_page_table1 is 4 bytes.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea 4f, %ecx
	jmp *%ecx

4:
	# Setup stack
	movl $stack_top, %esp
	xor %ebp, %ebp          # Set EBP to null
	push %ebp               # Push null return address to signal end of stack. Useful for debugging.

    # Push multiboot paramaters to stack for call to kernel_start
    # Push magic
    push %eax
    # Convert EAX (Grub multiboot structure) to a virtual address. Then push
    add $0xC0000000, %ebx  
    push %ebx

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_start

	# Hang if kernel_main unexpectedly returns.
5:	cli
    hlt
	jmp 5b
.size _start, . - _start


# GDT functions