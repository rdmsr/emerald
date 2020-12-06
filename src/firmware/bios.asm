bits 64

global shutdown

shutdown:
  mov ax, 0x1000
  mov ax, ss
  mov sp, 0xf000
  mov ax, 0x5307
  mov bx, 0x0001
  mov cx, 0x0003
  int 0x15
  
section .bss
resb 8192; 8KB for stack
section .text
stack_space: