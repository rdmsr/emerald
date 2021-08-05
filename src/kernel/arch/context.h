/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_CONTEXT_H
#define KERNEL_CONTEXT_H
#include <arch/descriptors/interrupts.h>
#include <emerald/std.h>

typedef struct
{
    Stack regs;
    uint64_t *pagemap;
} Context;

void context_load(Context *ctx, Stack *rsp);
void context_init(Context *ctx, uintptr_t ip, uintptr_t sp);

#endif
