/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_TASKING_H
#define KERNEL_TASKING_H
#include <arch/context.h>
#include <arch/descriptors/interrupts.h>
#include <emerald/alloc.h>
#include <emerald/ds/vec.h>
#include <emerald/str.h>

typedef enum
{
    RUNNABLE,
    DEAD,
    BLOCKING
} TaskState;

typedef struct
{
    String name;
    uintptr_t sp;
    u32 pid;
    Context *ctx;
    int burst_time, time_ran, time_start;
    TaskState state;
} Task;

struct schedule
{
    Task *data;
    struct schedule *next;
    uint16_t time;
};

Task *task_create(String name, int burst_time, uintptr_t ip);

void task_set_state(Task *task, TaskState state);

#endif
