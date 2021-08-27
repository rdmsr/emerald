/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_PCI_H
#define KERNEL_PCI_H
#include <emerald/std.h>

typedef struct
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    u8 function;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class;
    uint8_t cache_lsize;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    u8 bus;
} PCIDevice;

void pci_init(void);

#endif
