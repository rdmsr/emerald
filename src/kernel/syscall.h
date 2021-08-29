/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H
#include <arch/descriptors/interrupts.h>
#include <emerald/std.h>
#include <syscalls/syscalls.h>

BeSysRet syscall_handler(Stack *stack);

#endif
