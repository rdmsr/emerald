/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "syscall.h"
#include <emerald/log.h>

BeSysRet sys_log(char *args)
{
    log_syscall("{}", args);
    
    return 0;
}

BeSyscallFn *syscalls[BE_SYSCALL_COUNT] = {
    [BE_SC_LOG] = sys_log,
};

BeSysRet syscall_handler(Stack *stack)
{
    if ((int)stack->rax >= BE_SYSCALL_COUNT)
    {
        return -1;
    }

    return syscalls[stack->rax](stack->rbx, stack->rcx, stack->rdx, stack->rsi);
}
