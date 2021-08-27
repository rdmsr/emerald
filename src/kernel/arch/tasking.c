/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "tasking.h"
#include "arch/memory/pmm.h"
#include "arch/memory/vmm.h"
#include "emerald/alloc.h"
#include <emerald/debug.h>
#include <emerald/log.h>

static int current_pid = 0;

Task *task_create(String name, int burst_time, uintptr_t ip)
{
    Task *task = alloc_malloc(sizeof(Task));

    task->pid = current_pid++;
    task->name = name;
    task->burst_time = burst_time;
    task->time_ran = burst_time;
    task->state = DEAD;

    task->ctx = alloc_malloc(sizeof(Context));
    //task->ctx->pagemap = vmm_create_space();

    task->ctx->regs.rip = ip;

    // We're using this so we can check if it's NULL later on
    void *stack_alloc = alloc_malloc_end(4096);

    assert_not_null(task && stack_alloc);

    task->sp = (uintptr_t)stack_alloc + MEM_PHYS_OFFSET;

    log("Created task {} with pid {} stack {p}", task->name, task->pid, task->sp);

    return task;
}

void task_set_state(Task *task, TaskState state)
{
    task->state = state;
}
