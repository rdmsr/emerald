/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_APIC_H
#define KERNEL_APIC_H
#include <emerald/std.h>
#include <fw/acpi.h>

#define LAPIC_SPURIOUS_ALL 0xff
#define LAPIC_SPURIOUS_ENABLE_APIC 0x100

#define LAPIC_ICR_CPUID_OFFSET 24

#define LAPIC_ICR_CLEAR_INIT_LEVEL (1 << 14)

#define LAPIC_ICR_DEST_INIT (5 << 8)
#define LAPIC_ICR_DEST_SEND_IPI (6 << 8)

#define IOAPIC_REG_OFFSET (0)
#define IOAPIC_VALUE_OFFSET (16)


enum lapic_reg
{
    LAPIC_CPU_ID = 0x20,
    LAPIC_REG_EOI = 0x0b0,
    LAPIC_REG_SPURIOUS = 0x0f0,
    LAPIC_REG_ICR0 = 0x300,
    LAPIC_REG_ICR1 = 0x310,
    LAPIC_REG_LVT_TIMER = 0x320,
    LAPIC_REG_TIMER_INITCNT = 0x380,
    LAPIC_REG_TIMER_CURRCNT = 0x390,
    LAPIC_REG_TIMER_DIV = 0x3e0,
};

enum apic_timer_reg
{
    APIC_TIMER_REG = 0x320,
    APIC_TIMER_INITIAL_COUNT = 0x380,
    APIC_TIMER_CURRENT_COUNT = 0x390,
    APIC_TIMER_DIVIDER = 0x3e0,
    APIC_LVT_INT_MASKED = 0x10000,
    APIC_LVT_TIMER_MODE_PERIODIC = 0x20000,
};

enum ioapic_reg
{
    IOAPIC_REG_VERSION = 0x1,
    IOAPIC_REG_REDIRECT_BASE = 0x10,
};

void apic_timer_initialize(void);
void ioapic_initialize(void);
void apic_initialize(void);
void ioapic_redirect_irq(uint8_t lapic_id, uint8_t irq, uint8_t vect, bool status);

void apic_eoi(void);
#endif
