/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "syscall.h"
#include <emerald/log.h>

BeSysRet _be_log(MAYBE_UNUSED char *args)
{
    log_syscall("{}", args);
    
    return 0;
}

BeSysRet be_log(char *args)
{
    return be_syscall(BE_SC_LOG, (BeArg)args, 0, 0, 0);
}

BeSyscallFn *syscalls[BE_SYSCALL_COUNT] = {
    [BE_SC_LOG] = _be_log,
};

BeSysRet be_syscall(BeSyscall syscall, BeArg arg1, BeArg arg2, BeArg arg3, BeArg arg4)
{

    asm volatile("int $0x42"
                 :
                 : "a"(syscall), "b"(arg1), "c"(arg2), "d"(arg3), "r"(arg4));

    return 0; // FIXME: return the syscall's return code
}

BeSysRet syscall_handler(Stack *stack)
{
    if ((int)stack->rax >= BE_SYSCALL_COUNT)
    {
        return -1;
    }

    return syscalls[stack->rax](stack->rbx, stack->rcx, stack->rdx, stack->rsi);
}
