global init_context_switch
global end_context_switch
extern execute_tasks

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
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
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

init_context_switch:
        pushaq
	mov rdi,rsp 		; arguments of the function
	call execute_tasks
	iretq
end_context_switch:	
	mov rsp,[rdi] 		;moves the adress of rdi into rsp
	
        popaq
	iretq
