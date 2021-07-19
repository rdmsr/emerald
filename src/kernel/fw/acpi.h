/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_ACPI_H
#define KERNEL_ACPI_H
#include <boot/stivale2.h>
#include <emerald/ds/vec.h>
#include <emerald/std.h>

typedef struct
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} SDT;

typedef struct
{
    SDT descriptor;
    uint32_t sptr[];
} RSDT;

typedef struct
{
    char sign[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t rev;
    uint32_t rsdt;
} RSDP;

typedef struct
{
    uint8_t type;
    uint8_t length;
} MADTRecord;

typedef struct
{

    SDT base;
    uint32_t lapic;
    uint32_t flags;
    MADTRecord records[];
} MADT;

struct madt_ioapic_record
{
    MADTRecord base;
    uint8_t apic_id;
    uint8_t reserved;
    uint32_t address;
    uint32_t gsi;
};

struct madt_iso_record
{
    MADTRecord base;
    uint8_t bus_source;
    uint8_t irq_source;
    uint16_t flags;
    uint32_t gsi;
};

struct madt_lapic_record
{
    SDT base;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
};

typedef vec_t(struct madt_ioapic_record *) vec_ioapic_t;
typedef vec_t(struct madt_iso_record *) vec_iso_t;
typedef vec_t(struct madt_lapic_record *) vec_lapic_t;

uintptr_t acpi_get_lapic();

vec_ioapic_t acpi_get_ioapics(void);
vec_iso_t acpi_get_isos(void);
vec_lapic_t acpi_get_lapics(void);

void acpi_initialize(struct stivale2_struct *boot_info);

#endif
