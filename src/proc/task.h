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
#ifndef TASK_H
#define TASK_H
#pragma once
#include <mem/virtual/vmm.h>
#include <stdint.h>
enum state
{
    CANCELLED,
    RUNNING,
    STOPPED,
    IDLING

};
typedef struct regs64_t
{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rsp, rip;
    uint64_t ss, cs, fs;
    uint64_t rflags;
} regs64_t;

typedef struct
{
    uint8_t priority;
    uint32_t time_slice;
    regs64_t registers;
    int ring;
    void *kernel_stack;
} thread_t;

typedef struct process_struct
{
    int id;
    thread_t* thread;
    pagemap_t *pagemap;
    char *name;
    int state;
} process_t;

process_t EmeraldProc_Task_create_process(char* name,void (*function)(),int id, uint64_t priority,uint64_t rsp, uint8_t ring);
void EmeraldProc_Scheduler_schedule_task();
void EmeraldProc_Scheduler_give_cpu();
void EmeraldProc_Scheduler_init();
#endif
