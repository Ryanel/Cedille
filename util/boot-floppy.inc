msgprefix db 'fboot: ', 0
errormsg db 'There was a error during boot!', 13, 10, 0
%macro Print 1
BiosPrint msgprefix
BiosPrint %1
%endmacro

ch_loop:lodsb
   or al, al
   jz done
   mov ah, 0x0E
   int 0x10
   jmp ch_loop
done:
	ret
%macro BiosPrint 1
mov si, word %1
call ch_loop
%endmacro
	
fail_error:
	Print errormsg
	cli
	jmp halt