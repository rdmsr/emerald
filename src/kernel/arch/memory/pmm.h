/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include <emerald/log.h>
#include <emerald/macros.h>
#include <emerald/std.h>
#include <boot/stivale2.h>
#include <emerald/mem.h>

/* 4 KB per page */
#define PAGE_SIZE 4096
#define MEM_PHYS_OFFSET ((uintptr_t)0xffff800000000000)
#define KERNEL_BASE ((uintptr_t)0xffffffff80000000)

void pmm_initialize(struct stivale2_struct* boot_info);

void *pmm_allocate(size_t pages);
void *pmm_allocate_zero(size_t pages);
void pmm_free(void* addr, size_t pages);
void print_bitmap(int n);
#endif
