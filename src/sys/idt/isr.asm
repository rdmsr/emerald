
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr10
global isr11
global isr12
global isr13
global isr14
global isr16
global isr17
global isr18
global isr19
global isr20
global isr30
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


	
isr0:
	call EmeraldProc_PIT_start_timer
        mov al, 0x20
	out 0x20, al
	iretq
isr1:
	pushaq
	cld
	call EmeraldDevices_keyboard_Keyboard_handler_main
	popaq
	mov al, 0x20
	out 0x20, al
	iretq
isr2:
isr3:
isr4:
isr5:
isr6:
isr7:
isr8:
isr10:
isr11:
isr12:
isr13:
isr14:
isr16:
isr17:
isr18:
isr19:
isr20:
isr30:
