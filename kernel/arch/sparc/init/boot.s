.align 16
.section .data
tstack:
	.fill	0x500
.section .text
.global _start
.extern kernel_entry
_start:
	set	tstack, %o1
	save	%o1, 0x500-4, %sp
	call kernel_entry
		nop
	call halt
		nop
! Expects address offset in %o3
do_firmware_call:
	ret

halt:
	call halt                      ! loop forever
		nop
