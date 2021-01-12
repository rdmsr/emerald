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
#include <debug-utilities/logger.h>
#include <liballoc/alloc.h>
#include <libk.h>
struct process_struct *process_queue;
process_t *running;
process_t *idle;
int size_of_queue = 0;
regs64_t kernel_registers = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0x8, 0, 0x202};
regs64_t user_registers = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x23, 0x1B, 0, 0x202};

void queue_append(process_t process)
{
    process_queue[size_of_queue - 1] = process;
}

thread_t *create_thread(void (*function)(), uint64_t rsp, uint8_t ring)
{
    /* allocate the new thread */
    thread_t *new_thread = kcalloc(sizeof(thread_t));
    new_thread->kernel_stack = (void *)kcalloc(0x1000) + 0x1000;
    /* If the ring is for userspace, give it the default registers for userspace threads*/
    if (ring == 3)
    {
        new_thread->registers = user_registers;
    }
    else
    {
        new_thread->registers = kernel_registers;
    }
    new_thread->ring = ring;
    new_thread->registers.rip = (uint64_t)function;
    new_thread->registers.rsp = rsp;
    return new_thread;
}

process_t EmeraldProc_Task_create_process(char *name, void (*function)(), int id, uint64_t priority, uint64_t rsp, uint8_t ring)
{
    process_t process;
    pagemap_t *m_pagemap = m_pagemap;
    thread_t *thread = create_thread(function, rsp, ring);
    EmeraldMem_VMM_create_pagemap(m_pagemap);

    process.name = name;
    process.state = IDLING;
    process.id = id;
    process.thread = thread;
    process.thread->priority = priority;

    process.pagemap = m_pagemap;
    EmeraldMem_VMM_map_page(m_pagemap, lower_half(0x10000000000), 0x10000000000, 0b11);
    size_of_queue++;
    queue_append(process);
    log(INFO, "Created process called: %s with id: %d", name, id);

    return process;
}
/* This is probably not the best implementation but it works for now, bubble sort has a complexity of O(n2) which is not the best*/
void EmeraldProc_Scheduler_schedule_task()
{
    EmeraldProc_PIT_init(1000);
    /* Puts the process with the highest priority at the start of the queue */
    for (int i = 0; i < size_of_queue - 1; i++)
    {
        if (process_queue[i].thread->priority < process_queue[i + 1].thread->priority)
        {
            process_t temp = process_queue[i];
            process_queue[i] = process_queue[i + 1];
            process_queue[i + 1] = temp;

            log(INFO, "Swapped process named %s with process named %s", process_queue[i].name, process_queue[i + 1].name);
            kassert(process_queue[i].thread->priority > process_queue[i + 1].thread->priority);
        }
    }
}
void save_state(thread_t *thread)
{
    asm("push %0"
        : "=r"(thread->registers.rsp));
}
void EmeraldProc_Scheduler_give_cpu()
{
    for (int i = 0; i < size_of_queue; i++)
    {
        process_queue[i].state = RUNNING;
        if (process_queue[i].state == RUNNING)
        {

            save_state(process_queue[i].thread);
            asm("pop %rsp");
            process_queue[i].state = IDLING;
        }
    }
}
void test()
{
    log(INFO, "Function called!");
}
void EmeraldProc_Scheduler_init()
{
    EmeraldProc_Task_create_process("Kernel", test, 0, 10, (uint64_t)kmalloc(0x4000) + 0x4000, 0);
    EmeraldProc_Task_create_process("2Kernel", test, 1, 20, (uint64_t)kmalloc(0x4000) + 0x4000, 0);
    EmeraldProc_Scheduler_schedule_task();

    //EmeraldProc_Task_create_process(1, 30, 0xFFF, thread, "garbage");

    EmeraldProc_Scheduler_give_cpu();
}
