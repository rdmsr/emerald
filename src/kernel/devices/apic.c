/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "apic.h"
#include <devices/pit.h>
#include <emerald/debug.h>

static uintptr_t lapic_addr = 0;

static uint32_t lapic_read(uint32_t reg)
{
    kassert(lapic_addr != 0);
    return *((volatile uint32_t *)(lapic_addr + MEM_PHYS_OFFSET + reg));
}

static void lapic_write(uint32_t reg, uint32_t value)
{
    *((volatile uint32_t *)(lapic_addr + MEM_PHYS_OFFSET + reg)) = value;
}

void apic_timer_initialize(void)
{
    lapic_addr = acpi_get_lapic();

    lapic_write(APIC_TIMER_DIVIDER, 0x3);
    lapic_write(APIC_TIMER_INITIAL_COUNT, 0xFFFFFFFF);

    pit_usleep(10);
    lapic_write(APIC_TIMER_REG, APIC_LVT_INT_MASKED);

    uint32_t tickIn10ms = 0xFFFFFFFF - lapic_read(APIC_TIMER_CURRENT_COUNT);

    lapic_write(APIC_TIMER_REG, 32 | APIC_LVT_TIMER_MODE_PERIODIC);
    lapic_write(APIC_TIMER_DIVIDER, 0x3);
    lapic_write(APIC_TIMER_INITIAL_COUNT, tickIn10ms);

    log("Initialized apic timer");
    
}
