/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H
#include <arch/descriptors/interrupts.h>
#include <emerald/std.h>

/* Syscalls:
        * rax: number
        * rbx Arg1
	* rcx Arg2
	* rdx Arg3
	* rsi Arg4
	*/

typedef int BeSysRet;

typedef uint64_t BeArg;

typedef BeSysRet BeSyscallFn();

#define SYSCALL(NAME) BeSysRet sys_##NAME

#define FOREACH_SYSCALLS(SYSCALL) \
    SYSCALL(LOG)

typedef enum
{
#define ITER(SYSCALL) BE_SC_##SYSCALL,
    FOREACH_SYSCALLS(ITER)
#undef ITER

        BE_SYSCALL_COUNT
} BeSyscall;

BeSysRet be_syscall(BeSyscall syscall, BeArg arg1, BeArg arg2, BeArg arg3, BeArg arg4);

BeSysRet syscall_handler(Stack *stack);

typedef struct
{
    char const *message;
  //size_t size;
} BeLogArgs;


BeSysRet be_log(char *args);

#endif
