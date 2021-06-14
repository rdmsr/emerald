	org 0x7c00
init:	jmp short .jmp	; reserved for jump instruction, or other purposes
	nop
	nop		; fill up gap
	db '_ECH_FS_'	; echidnaFS signature
	dq 0		; total block count
	dq 0		; length of the main directory in blocks (usually 5% of the total blocks)
	dq 0		; bytes per block (MUST be a multiple of 512)
.jmp:	push cs
	pop ds
	push cs
	pop es
	mov si, error_msg
	mov ah, 0x0e
	mov cx, 0x0001
	mov bx, 0x000e
.loop:	lodsb
	test al, al
	je short .hlt
	int 0x10
	jmp short .loop
.hlt:	cli
	hlt
	jmp short .hlt
error_msg:	db "Not a bootable VBR. This is a echidnaFS partition!", 0x0a, 0x0d, 0x00
times 510 - ($ - $$) nop
db 0x55, 0xaa
