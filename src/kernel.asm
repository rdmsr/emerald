bits 64
section .text
global start
global isr
global panic
global isr_irq_master
global isr_irq_slave
global shutdown
extern kmain
extern EmeraldProc_PIT_start_timer
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
isr:
	pushaq
	cld
	call EmeraldDevices_keyboard_Keyboard_handler_main
	popaq
	mov al, 0x20
	out 0x20, al
	iretq
isr_irq_master:
	call EmeraldProc_PIT_start_timer
	mov al, 0x20
	out 0x20, al
	iretq

isr_irq_slave:
	mov al, 0x20
	out 0xa0, al
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
