global _user_jump
extern user_jump
_user_jump:
	mov ax, 0x23
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
 
	mov rax, rsp
	
	push 0x23
	
	push rax		
	
	pushfq
	
	push 0x1b
	
	push user_jump
	
	iretq
