bits 64
section .text
global start
global read_port
global write_port
global load_idt
global isr
global isr_irq_master
global isr_irq_slave
extern gdtr
extern kmain 		;this is defined in the c file
extern EmeraldDevices_keyboard_Keyboard_handler_main
%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro
%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro
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
isr:
	pushaq
	cld
	call EmeraldDevices_keyboard_Keyboard_handler_main
	popaq
	mov al, 0x20
	out 0x20, al
	iretq
isr_irq_master:
	mov al, 0x20
	out 0x20, al
	iretq

isr_irq_slave:
	mov al, 0x20
	out 0xA0, al
	out 0x20, al
	iretq
	
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
