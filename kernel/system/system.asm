bits 64
section .text

global isr
global isr_irq_master
global isr_irq_slave
global pit_handler
global keyboard_handler
extern PIT_add_ticks
extern Keyboard_main
	
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

isr:


isr_irq_master:
	mov al, 0x20
	out 0x20, al
	iretq

isr_irq_slave:
	mov al, 0x20
	out 0xa0, al
	out 0x20, al
	iretq

pit_handler:
	pushaq
	call PIT_add_ticks
	popaq
	iretq	
keyboard_handler:
	pushaq
	cld
	call Keyboard_main
	popaq
	iretq

