/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "syscall.h"
#include <emerald/log.h>

BeSysRet be_log(char *args)
{
    return be_syscall(BE_SC_LOG, (BeArg)args, 0, 0, 0);
}

BeSysRet be_syscall(BeSyscall syscall, BeArg arg1, BeArg arg2, BeArg arg3, BeArg arg4)
{

    BeSysRet ret = 0;
    
    asm volatile("int $0x42"
                 : "=a"(ret)
                 : "a"(syscall), "b"(arg1), "c"(arg2), "d"(arg3), "r"(arg4));

    return ret;
}
