/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "context.h"
#include <arch/memory/vmm.h>
#include <emerald/liballoc/liballoc.h>

#define RFLAGS_CARRY_FLAG 1 << 9
#define RFLAGS_INTERRUPT_FLAG 1 << 1

void context_load(Context *ctx, Stack* rsp)
{
  *rsp = ctx->regs;
  vmm_load_pagemap(ctx->pagemap);
}

void context_init(Context *ctx, uintptr_t ip, uintptr_t sp)
{
  ctx->regs.rip = ip;
  ctx->regs.cs = 0x8;
  ctx->regs.ss = 0x10;
  ctx->regs.rflags = RFLAGS_CARRY_FLAG | RFLAGS_INTERRUPT_FLAG; /* 0x202 or 0b1000000010 */
  ctx->regs.rsp = sp;
}
