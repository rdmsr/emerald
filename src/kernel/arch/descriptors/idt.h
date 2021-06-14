/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <emerald/log.h>
#include <emerald/std.h>

typedef struct __attribute__((packed))
{
    uint16_t size;
    uint64_t addr;
} IDTPointer;

typedef struct __attribute__((packed))
{
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero;
} IDTDescriptor;

void idt_initialize();

#endif
