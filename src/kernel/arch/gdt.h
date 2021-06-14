/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

#include <emerald/std.h>
#include <emerald/log.h>

typedef struct __attribute__((packed))
{
  uint16_t limit;
  uintptr_t base;
} GDTPointer;

typedef struct __attribute__((packed))
{
    uint16_t limit_low16;
    uint16_t base_low16;
    uint8_t base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high8;
} GDTDescriptor;

void gdt_init();

#endif
