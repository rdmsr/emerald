bits 64
section .text
global start
global read_port
global write_port
global load_idt
extern gdtr
extern kmain 		;this is defined in the c file
extern keyboard_handler_main

read_port:
	mov edx, [rsp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret


load_idt:
	mov edx, [rsp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret
start:
	cli 				;block interrupts
	mov rsp, stack_space
	call kmain
	hlt 				;halt the CPU

section .bss
resb 8192; 8KB for stack
section .text
stack_space:

Cursor:
VGA.Width equ 80
 
.SetCoords:
; input bx = x, ax = y
; modifies ax, bx, dx
 
	mov dl, VGA.Width
	mul dl
	add bx, ax
 
.SetOffset:
; input bx = cursor offset
; modifies al, dx
 
	mov dx, 0x03D4
	mov al, 0x0F
	out dx, al
 
	inc dl
	mov al, bl
	out dx, al
 
	dec dl
	mov al, 0x0E
	out dx, al
 
	inc dl
	mov al, bh
	out dx, al
	ret
