/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_BOOT_H
#define KERNEL_BOOT_H
#include <boot/stivale2.h>
#include <emerald/std.h>

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id);

#endif
