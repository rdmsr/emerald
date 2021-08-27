/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "sched.h"
#include "arch/arch.h"
#include "devices/pit.h"
#include "emerald/debug/assert.h"
#include "emerald/macros.h"
#include "emerald/str/str.h"
#include <emerald/debug.h>

static u32 lock = 0;
static u64 tick = 0;

static vec_t(Task *) tasks = {};
static struct schedule *begin, *current, kernel;
static int current_index = 1;
static size_t time_slice = 2;

Task *sched_current()
{
    return current->data;
}

MAYBE_UNUSED static struct schedule *sched_current_next()
{
    return current->next;
}

u64 sched_tick()
{
    return tick;
}

MAYBE_UNUSED static size_t active_time(Task *task)
{
    return sched_tick() - task->time_start;
}

void sched_initialize(void)
{
    vec_init(&tasks);

    current = alloc_malloc(sizeof(struct schedule));

    kernel.data = alloc_malloc(sizeof(Task));

    kernel.data->ctx = alloc_malloc(sizeof(Context));

    begin = current;

    log("Initialized scheduler");
}

static void sched_add(Task *task)
{
    vec_push(&tasks, task);

    assert_truth(str_cmp(tasks.data[tasks.length - 1]->name, task->name) == true);

    current->data = tasks.data[0];
}

MAYBE_UNUSED static void sched_remove(Task *task)
{
    vec_remove(&tasks, task);
}

void sched_start(Task *task)
{
    lock_acquire(&lock);

    assert_truth(task->state != RUNNABLE);

    task_set_state(task, RUNNABLE);

    context_init(task->ctx, task->ctx->regs.rip, task->sp);

    sched_add(task);

    log("Starting task {} ({})", task->name, task->pid);

    lock_release(&lock);
}

void sched_stop(Task *task)
{
    lock_acquire(&lock);

    if (task->state == DEAD)
    {
        lock_release(&lock);
        return;
    }

    task_set_state(task, DEAD);

    lock_release(&lock);
}

void sched_next()
{
    current->next = alloc_malloc(sizeof(struct schedule));

    sched_current_next()->data = tasks.data[current_index];

    current_index++;

    if (sched_current_next()->data->state == RUNNABLE)
    {
        current = current->next;
    }
}

void sched_schedule(MAYBE_UNUSED Stack *stack)
{
    // Save current context in the `kernel` task
    context_save(kernel.data->ctx, stack);
    
    tick++;

    lock_acquire(&lock);

    assert_not_null(current);

    current->data->time_start = sched_tick();

    if (current->data->state == RUNNABLE)
    {
        int prev_ticks = sched_tick();

        // load the task's context

        context_load(current->data->ctx, stack);
	
        while (sched_tick() - prev_ticks < time_slice)
        {
            tick++;
        }

        // go back to the kernel's context

        log("ran task {} for {} ticks, rip = {p}, rsp = {p}", current->data->name, sched_tick() - prev_ticks, current->data->ctx->regs.rip, current->data->ctx->regs.rsp);

        //context_load(kernel.data->ctx, stack);
	
        if (current_index == tasks.length)
        {
            current_index = 1;

            if (begin->data->state == RUNNABLE)
            {
                current = begin;
            }
        }

        else
        {
            sched_next();
        }
    }

    lock_release(&lock);

}
