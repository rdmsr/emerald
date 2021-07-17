/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "acpi.h"
#include <arch/cpuid.h>
#include <emerald/log.h>
#include <emerald/str.h>

static uintptr_t lapic_addr = 0;

uintptr_t acpi_get_lapic()
{
    return lapic_addr;
}

void acpi_initialize(struct stivale2_struct *boot_info)
{

    if (!cpuid_has_msr())
    {
        struct stivale2_struct_tag_rsdp *rsdp_info = stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_RSDP_ID);

        RSDP *rsdp = (RSDP *)rsdp_info->rsdp;
        RSDT *rsdt = (RSDT *)((uintptr_t)rsdp->rsdt + MEM_PHYS_OFFSET);

        size_t entries = (rsdt->descriptor.length - sizeof(rsdt->descriptor)) / 4;

        size_t i;

        for (i = 0; i < entries; i++)
        {
            SDT *h = (SDT *)(rsdt->sptr[i] + MEM_PHYS_OFFSET);

            if (str_ncmp(make_str(h->signature), make_str("APIC"), 4) == 0)
            {
                MADT *madt = (MADT *)h;

                if (lapic_addr == 0)
                {
                    log("Found lapic: {p}", madt->lapic);

                    lapic_addr = madt->lapic;
                }
            }
        }
    }

    else
    {
        lapic_addr = asm_read_msr(0x1b) & 0xfffff000;
        log("Found lapic: {p}", lapic_addr);
    }

    log("ACPI initialized");
}
