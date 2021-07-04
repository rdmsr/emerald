/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H
#include <devices/com.h>
#include <emerald/io/rw.h>
#include <boot/boot.h>

Writer *arch_debug_writer();
void arch_initialize_descriptors(void);
void arch_initialize_memory(struct stivale2_struct *bootinfo);

#endif
