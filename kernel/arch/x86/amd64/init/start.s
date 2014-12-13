.global _start
_start:
	movl $0x07690748,0xb8000
halt:
	cli
	hlt
	jmp halt
