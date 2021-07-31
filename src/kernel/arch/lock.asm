global lock_acquire
global lock_release

lock_acquire:
    lock bts dword [rdi], 0
    jc .spin 
    ret 

.spin:
    pause 
    test dword [rdi], 1
    jnz .spin 
    jmp lock_acquire

lock_release:
    mov dword [rdi], 0
    ret
