/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_TASKING_H
#define KERNEL_TASKING_H
#include <arch/context.h>
#include <arch/descriptors/interrupts.h>
#include <emerald/ds/vec.h>
#include <emerald/alloc.h>
#include <emerald/std.h>
#include <emerald/str/str.h>

typedef struct
{
    String name;
    uintptr_t sp;
    u32 pid;
    Context ctx;
} Task;


struct schedule
{
    Task *idle;
    Task *current;
    Task *next;
};

void scheduler_schedule_and_switch(void);

Task *task_create(String name, uintptr_t ip);

#endif
