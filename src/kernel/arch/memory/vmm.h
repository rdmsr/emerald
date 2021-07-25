/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_VMM_H
#define KERNEL_VMM_H
#include <arch/memory/pmm.h>

#define PML_ENTRY(addr, offset) (size_t)(addr & ((uintptr_t)0x1ff << offset)) >> offset;

void vmm_initialize(struct stivale2_struct *stivale2_struct);
void vmm_load_pagemap(uint64_t* pagemap);
#endif
