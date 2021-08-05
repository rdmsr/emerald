/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "tasking.h"
#include <emerald/debug.h>
#include <emerald/log.h>

size_t current_tick = 0;
size_t current_pid = 0;
static vec_t(Task *) tasks = {};

Task *task_create(String name, uintptr_t ip)
{
    Task *task = alloc_malloc(sizeof(Task));

    task->pid = current_pid++;
    task->name = name;

    // We're using this so we can check if it's NULL later on
    void *stack_alloc = alloc_malloc_end(4096);

    kassert(task && stack_alloc);

    task->sp = (uintptr_t)stack_alloc;

    context_init(&task->ctx, ip, task->sp);

    vec_push(&tasks, task);

    log("Created task {s} with pid {i} stack {p}", task->name, task->pid, task->sp);

    return task;
}


