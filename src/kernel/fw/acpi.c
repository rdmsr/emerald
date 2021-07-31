/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "acpi.h"
#include <arch/cpuid.h>
#include <emerald/debug.h>
#include <emerald/log.h>
#include <emerald/str.h>

/* NOTE
   This file is most likely broken (F in the chat)*/
static uintptr_t lapic_addr = 0;
static vec_ioapic_t ioapics;
static vec_iso_t isos;
static vec_lapic_t lapics;

uintptr_t acpi_get_lapic()
{
    return lapic_addr;
}

vec_ioapic_t acpi_get_ioapics()
{
    return ioapics;
}

vec_lapic_t acpi_get_lapics()
{
    return lapics;
}

vec_iso_t acpi_get_isos()
{
    return isos;
}

bool do_acpi_checksum(SDT *th)
{
    uint8_t sum = 0;

    for (uint32_t i = 0; i < th->length; i++)
        sum += ((char *)th)[i];

    return sum == 0;
}

void *get_table(String signature, RSDP *rsdp, RSDT *rsdt, XSDT *xsdt, int index)
{
    size_t entries;

    if (rsdp->rev <= 2)
        entries = (rsdt->descriptor.length - sizeof(rsdt->descriptor)) / 4;
    else
        entries = (xsdt->descriptor.length - sizeof(xsdt->descriptor)) / 8;

    int i = 0;
    SDT *h;

    for (size_t t = 0; t < entries; t++)
    {
        if (rsdp->rev <= 2)
            h = (SDT *)(uint64_t)(rsdt->sptr[t] + MEM_PHYS_OFFSET);
        else
            h = (SDT *)(uint64_t)(xsdt->sptr[t] + MEM_PHYS_OFFSET);

        if (str_ncmp(signature, make_str(h->signature), 4) == 0)
        {
            if (do_acpi_checksum(h) && i == index)
                return (void *)h;

            i++;
        }
    }

    return NULL;
}

void acpi_initialize(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_rsdp *rsdp_info = stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_RSDP_ID);

    RSDP *rsdp = (RSDP *)rsdp_info->rsdp;
    RSDT *rsdt = (RSDT *)((uintptr_t)rsdp->rsdt + MEM_PHYS_OFFSET);

    XSDT *xsdt = NULL;
    if (rsdp->rev >= 2)
        xsdt = (XSDT *)(rsdp->xsdt + MEM_PHYS_OFFSET);

    MADT *madt = get_table(make_str("APIC"), rsdp, rsdt, xsdt, 0);

    kassert(madt != NULL);

    vec_init(&ioapics);
    vec_init(&isos);
    vec_init(&lapics);

    if (!cpuid_has_msr())
    {
        log("Found lapic: {p}", madt->lapic);

        lapic_addr = madt->lapic;
    }

    else
    {
        lapic_addr = asm_read_msr(0x1b) & 0xfffff000;
        log("Found lapic: {p}", lapic_addr);
    }

    size_t i;

    /* FIXME: record->length is 0 for a certain reason */
    for (i = 0; i < (madt->base.length - sizeof(MADT));)
    {
        MADTRecord *record = (MADTRecord *)&madt->records[i];

        switch (record->type)
        {

        case 0:
            vec_push(&lapics, (struct madt_lapic_record *)record);
            break;
        case 1:
            vec_push(&ioapics, (struct madt_ioapic_record *)record);
            break;

        case 3:
        {
            vec_push(&isos, (struct madt_iso_record *)record);
            break;
        }

        default:
            break;
        }

        if (record->length == 0)
        {
            i++;
        }

        i += record->length;
    }

    log("ACPI initialized");
}
