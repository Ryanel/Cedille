[ORG 0x7c00]
[BITS 16]
jmp start

bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	DB 2
bpbRootEntries: 	DW 224
bpbTotalSectors: 	DW 2880
bpbMedia: 		DB 0xF0
bpbSectorsPerFAT: 	DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 		DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "

%include "boot-floppy.inc"

start:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov ds, ax
	mov sp, 0x7C00 ;Set Stack Pointer

	sti
	;Set 8x8
	mov ax, 0x1112
	xor bx,bx
	int 0x10

	;Print status messages
	Print bootmsg
loadsecondstage:
	Print flpmsg
reset_flp_controler:
	mov ah, 0
	mov dl, 0
	int 0x13
	jc reset_flp_controler
	mov		ax, 0x1000
	mov		es, ax
	xor		bx, bx
read_flp:
	mov		ah, 0x02				; function 2
	mov		al, 1					; read 1 sector
	mov		ch, 1					; we are reading the second sector past us, so its still on track 1
	mov		cl, 2					; sector to read (The second sector)
	mov		dh, 0					; head number
	mov		dl, 0					; drive number. Remember Drive 0 is floppy drive.
	int		0x13					; call BIOS - Read the sector
	jc		read_flp				; Error, so try again
execute_second:
	Print donemsg
	;jmp		0x1000:0x0
halt:
	cli
	hlt
	jmp halt

;Data
bootmsg db 'Loading...', 13, 10, 0
flpmsg db 'Loading Second Stage from Floppy', 13, 10, 0
donemsg db 'Done', 13, 10, 0
readmsg db 'Reading...', 13, 10, 0
;Bootable Footer + Padding
times 510-($-$$) db 0
db 0x55
db 0xAA
