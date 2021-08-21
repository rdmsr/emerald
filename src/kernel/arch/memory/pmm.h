/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include <boot/stivale2.h>
#include <emerald/functional.h>
#include <emerald/log.h>
#include <emerald/macros.h>
#include <emerald/mem.h>
#include <emerald/std.h>

/* 4 KB per page */
#define PAGE_SIZE 4096
#define MEM_PHYS_OFFSET ((uintptr_t)0xffff800000000000)
#define KERNEL_BASE ((uintptr_t)0xffffffff80000000)

void pmm_initialize(struct stivale2_struct *boot_info);

Maybe(voidp) pmm_allocate(size_t pages);

void *pmm_allocate_zero(size_t pages);

void pmm_free(void *addr, size_t pages);

void print_bitmap(int n);

size_t get_usable_pages();

#endif
