/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_INT_H
#define KERNEL_INT_H
#include <emerald/std.h>

typedef struct
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t intno;
    uint64_t err;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} Stack;

typedef struct stackframe
{
    struct stackframe *rbp;
    uint64_t rip;
} Stackframe;

uint64_t interrupts_handler(uint64_t rsp);
void interrupt_error_handler(Stack*);
u64 get_ticks();

void toggle_sched_init();

#endif
