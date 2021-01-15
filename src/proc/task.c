/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "task.h"
#include "PIT.h"
#include "lock.h"
#include <debug-utilities/logger.h>
#include <devices/keyboard/keyboard.h>
#include <liballoc/alloc.h>
#include <libk.h>
#include <sys/idt/idt.h>
spinlock_t scheduler_lock = 0;
uint16_t current_pid = 0;

/* Creates 3 lists , one for each priority */
static processlist_t task_lists[3] = {{.head = NULL, .current = NULL, .tail = NULL}};
static process_t *current_task;

extern void init_context_switch();
extern void end_context_switch(process_t *next_task);

uint16_t EmeraldProc_Task_create_process(void (*entrypoint)(), priority_t priority, int ring)
{
    spinlock_take(&scheduler_lock);

    /* An error occured */
    if (ring != 0)
    {
        log(ERROR, "TODO: Implement ring support");
        return -1;
    }
    /* Allocate new task and its kernel stack */

    process_t *new_task = kcalloc(sizeof(new_task));
    void *new_stack = kmalloc(4096) + 4096;

    /* Creates a new stack for the task */
    regs64_t *registers = new_stack - sizeof(regs64_t);

    /* Sets the value of cs,ss,rip,rsp and rdi */
    registers->cs = 0x08;
    registers->ss = 0x10;
    registers->rip = (uint64_t)entrypoint;
    registers->rsp = (uint64_t)new_stack;
    registers->rdi = current_pid;

    /* Initialize the new task */
    new_task->stack_top = registers;
    new_task->pid = current_pid;
    new_task->priority = priority;

    new_task->next = NULL;
    new_task->previous = NULL;

    /* Appends to the appropriate priority list */
    if (task_lists[priority].tail != NULL) /* is the list empty? if no, append the task to it*/
    {
        task_lists[priority].tail->next = new_task;
        new_task->previous = task_lists[priority].tail;
    }
    else /* If yes, initialize it */
    {
        log(DEBUG, "list is empty");
        task_lists[priority].head = new_task;
        task_lists[priority].current = new_task;
    }
    task_lists[priority].tail = new_task;
    task_lists[priority].size++;
    current_pid++;

    spinlock_release(&scheduler_lock);

    log(INFO, "Created new task with pid %d", new_task->pid);
    return new_task->pid;
}

void execute_tasks(regs64_t *context)
{
    /* scheduling algorithm by @chocabloc on github */

    if (!spinlock_try_take(&scheduler_lock))
        return;

    static uint64_t ticks = 4;

    processlist_t *list;

    while (true)
    {
        if (ticks % 6 == 0)
            list = &task_lists[BACKGROUND];
        else if (ticks % 2 == 0)
            list = &task_lists[REALTIME];
        else
            list = &task_lists[NORMAL];

        ticks++;
        if (list->current)
            break;
    }

    if (current_task)
        current_task->stack_top = context;

    list->current = list->current->next;
    if (!list->current)
        list->current = list->head;
    current_task = list->current;

    EmeraldPIC_sendEOI(0);
    spinlock_release(&scheduler_lock);

    end_context_switch(current_task);
}
void test()
{
    log(DEBUG, "Function called!");
}

void EmeraldProc_Scheduler_init()
{
    EmeraldProc_PIT_init(200);
    EmeraldProc_Task_create_process(test, BACKGROUND, 0);

    //EmeraldProc_Task_create_process(1, 30, 0xFFF, thread, "garbage");
}
