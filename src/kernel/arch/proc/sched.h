/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H
#include <emerald/std.h>
#include <arch/tasking.h>

void sched_initialize(void);
void sched_start(Task *task);
uint64_t sched_schedule(Stack* stack);

Task *sched_current(void);

#endif
