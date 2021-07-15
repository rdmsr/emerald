/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_ACPI_H
#define KERNEL_ACPI_H
#include <boot/stivale2.h>
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

uintptr_t acpi_get_lapic();
void acpi_initialize(struct stivale2_struct *boot_info);

#endif
