/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "context.h"
#include <arch/memory/vmm.h>
#include <emerald/debug.h>
#include <emerald/liballoc/liballoc.h>

#define RFLAGS_CARRY_FLAG 1 << 9
#define RFLAGS_INTERRUPT_FLAG 1 << 1

void context_load(Context *ctx, Stack *stack)
{
    asm_write_msr(0xC0000101, (uintptr_t)ctx);
    asm_write_msr(0xc0000102, (uintptr_t)ctx);

    *stack = ctx->regs;

    // vmm_load_pagemap(ctx->pagemap);
}

void context_save(Context *ctx, Stack *stack)
{
    ctx->regs = *stack;
}

void context_init(Context *ctx, uintptr_t ip, uintptr_t sp)
{
    ctx->regs.rip = ip;
    ctx->regs.cs = 0x8;
    ctx->regs.ss = 0x10;
    ctx->regs.rflags = RFLAGS_CARRY_FLAG | RFLAGS_INTERRUPT_FLAG; /* 0x202 or 0b1000000010 */
    ctx->regs.rsp = sp;
}
