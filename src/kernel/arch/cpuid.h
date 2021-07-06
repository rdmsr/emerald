/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_CPUID_H
#define KERNEL_CPUID_H
#include <arch/asm.h>
#include <emerald/std.h>

typedef struct
{

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} CPUID;

CPUID cpuid(uint32_t leaf, uint32_t subleaf, bool offset);

char* cpuid_get_vendor();

bool cpuid_has_lapic(void);

#endif
