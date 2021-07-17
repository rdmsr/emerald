/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include <arch/cpuid.h>
#include <emerald/debug.h>

CPUID cpuid(uint32_t leaf, uint32_t subleaf, bool offset)
{
    uint32_t cpuid_max;

    if (!offset)
    {
        __asm__ volatile("cpuid"
                         : "=a"(cpuid_max)
                         : "a"(leaf)
                         : "rbx", "rcx", "rdx");
    }

    if (offset)
    {
        __asm__ volatile("cpuid"
                         : "=a"(cpuid_max)
                         : "a"(leaf & 0x80000000)
                         : "rbx", "rcx", "rdx");
    }

    kassert(leaf < cpuid_max);

    CPUID result = {};

    __asm__ volatile("cpuid"
                     : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx)
                     : "a"(leaf), "c"(subleaf));

    return result;
}

char *cpuid_get_vendor()
{

    CPUID result = cpuid(0, 0, false);
    uint32_t ebx = result.ebx;
    uint32_t ecx = result.ecx;
    uint32_t edx = result.edx;

    static uint32_t a[3];

    a[0] = ebx;
    a[1] = edx;
    a[2] = ecx;

    return (char *)a;
}

bool cpuid_has_lapic(void)
{
    return (cpuid(1, 0, false).edx >> 9) & 1;
}

bool cpuid_has_msr(void)
{
  uint32_t CPUID_FLAG_MSR = 1 << 5;
  
  return (cpuid(1, 0, false).edx) & CPUID_FLAG_MSR;
}
