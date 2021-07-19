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

void *get_table(String signature, RSDT *rsdt)
{
    size_t entries = (rsdt->descriptor.length - sizeof(rsdt->descriptor)) / 4;

    size_t i;

    MADT *madt;

    for (i = 0; i < entries; i++)
    {
        SDT *h = (SDT *)(rsdt->sptr[i] + MEM_PHYS_OFFSET);

        if (str_ncmp(make_str(h->signature), signature, signature.size) == 0 )
        {
            madt = (MADT *)h;

            return madt;
        }
    }

    return NULL;
}

void acpi_initialize(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_rsdp *rsdp_info = stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_RSDP_ID);

    RSDP *rsdp = (RSDP *)rsdp_info->rsdp;
    RSDT *rsdt = (RSDT *)((uintptr_t)rsdp->rsdt + MEM_PHYS_OFFSET);

    MADT *madt = get_table(make_str("APIC"), rsdt);

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
	  log("f");
	  i++;
        }

        else
        {
            i += record->length;
        }
    }

    log("ACPI initialized");
}
