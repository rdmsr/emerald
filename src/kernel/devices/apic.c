/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "apic.h"
#include <devices/pit.h>
#include <emerald/debug.h>
#include <fw/acpi.h>

/* I hate APIC, you need to initialize 320984792834713897498127287642879530946 things to get it to work */

/* ============== LAPIC ====== */
static uintptr_t lapic_addr = 0;

static uint32_t lapic_read(uint32_t reg)
{
    assert_truth(lapic_addr != 0);
    return *((volatile uint32_t *)(lapic_addr + MEM_PHYS_OFFSET + reg));
}

static void lapic_write(uint32_t reg, uint32_t value)
{
    *((volatile uint32_t *)(lapic_addr + MEM_PHYS_OFFSET + reg)) = value;
}

void lapic_enable_spurious(void)
{
    lapic_write(LAPIC_REG_SPURIOUS, lapic_read(LAPIC_REG_SPURIOUS) | (1 << 8) | LAPIC_REG_SPURIOUS);
}

/* =========== IOAPIC ====== */
vec_ioapic_t ioapics;

static uint32_t ioapic_read(uintptr_t ioapic_address, size_t reg)
{
    assert_truth(ioapic_address != 0);
    *((volatile uint32_t *)(ioapic_address + MEM_PHYS_OFFSET + IOAPIC_REG_OFFSET)) = reg;
    return *((volatile uint32_t *)(ioapic_address + MEM_PHYS_OFFSET + IOAPIC_VALUE_OFFSET));
}

static void ioapic_write(uintptr_t ioapic_address, size_t reg, uint32_t data)
{
    assert_truth(ioapic_address != 0);
    *((volatile uint32_t *)(ioapic_address + MEM_PHYS_OFFSET + IOAPIC_REG_OFFSET)) = reg;
    *((volatile uint32_t *)(ioapic_address + MEM_PHYS_OFFSET + IOAPIC_VALUE_OFFSET)) = data;
}

static uint32_t get_gsi_count(uintptr_t ioapic_address)
{
    return (ioapic_read(ioapic_address, IOAPIC_REG_VERSION) & 0xff0000) >> 16;
}

static struct madt_ioapic_record *get_ioapic_by_gsi(uint32_t gsi)
{
    for (int i = 0; i < ioapics.length; i++)
    {
        struct madt_ioapic_record *ioapic = ioapics.data[i];
        if (ioapic->gsi <= gsi && ioapic->gsi + get_gsi_count(ioapic->address + MEM_PHYS_OFFSET) > gsi)
        {
            return ioapic;
        }
    }

    return NULL;
}

void ioapic_initialize(void)
{
    ioapics = acpi_get_ioapics();
}

void ioapic_redirect_gsi(uint8_t lapic_id, uint32_t gsi, uint8_t vec, uint16_t flags, bool status)
{
    size_t io_apic = get_ioapic_by_gsi(gsi)->apic_id;

    uint64_t redirect = vec;

    // Active high(0) or low(1)
    if (flags & 2)
    {
        redirect |= (1 << 13);
    }

    // Edge(0) or level(1) triggered
    if (flags & 8)
    {
        redirect |= (1 << 15);
    }

    if (!status)
    {
        // Set mask bit
        redirect |= (1 << 16);
    }

    // Set target APIC ID
    redirect |= ((uint64_t)lapic_id) << 56;
    uint32_t ioredtbl = (gsi - ioapics.data[io_apic]->gsi) * 2 + 16;

    ioapic_write(io_apic, ioredtbl + 0, (uint32_t)redirect);
    ioapic_write(io_apic, ioredtbl + 1, (uint32_t)(redirect >> 32));
}

void ioapic_redirect_irq(uint8_t lapic_id, uint8_t irq, uint8_t vect, bool status)
{
    auto isos = acpi_get_isos();
    for (int i = 0; i < isos.length; i++)
    {
        if (isos.data[i]->irq_source == irq)
        {
            ioapic_redirect_gsi(lapic_id, vect, isos.data[i]->gsi,
                                isos.data[i]->flags, status);
            return;
        }
    }

    ioapic_redirect_gsi(lapic_id, vect, irq, 0, status);
}

/* =========== APIC ======= */
void apic_send_ipi(uint8_t lapic_id, uint8_t vector)
{
    lapic_write(LAPIC_REG_ICR1, ((uint32_t)lapic_id) << 24);
    lapic_write(LAPIC_REG_ICR0, vector);
}

void apic_eoi(void)
{
    lapic_write(LAPIC_REG_EOI, 0);
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

void apic_initialize()
{
    lapic_addr = acpi_get_lapic();
    lapic_enable_spurious();
    ioapic_initialize();
}
