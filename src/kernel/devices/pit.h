/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_PIT_H
#define KERNEL_PIT_H

#include <emerald/std.h>
#include <emerald/log.h>

void pit_initialize(int hz);
void pit_usleep(uint16_t ms);

#endif
