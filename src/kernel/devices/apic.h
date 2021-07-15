/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_APIC_H
#define KERNEL_APIC_H
#include <emerald/std.h>
#include <fw/acpi.h>

enum LAPIC_REG 
{
    LAPIC_CPU_ID = 0x20,
    LAPIC_EOI = 0x0b0,
};

enum APIC_TIMER
{
    APIC_TIMER_REG = 0x320,
    APIC_TIMER_INITIAL_COUNT = 0x380,
    APIC_TIMER_CURRENT_COUNT = 0x390,
    APIC_TIMER_DIVIDER = 0x3e0,
    APIC_LVT_INT_MASKED = 0x10000,
    APIC_LVT_TIMER_MODE_PERIODIC = 0x20000,
};

void apic_timer_initialize(void);

#endif
