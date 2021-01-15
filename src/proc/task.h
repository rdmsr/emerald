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

typedef enum
{
    REALTIME,
    NORMAL,
    BACKGROUND
} priority_t;

typedef struct {
  uint64_t r15,r14,r13,r12,r11,r10,r9,r8,rbp,rdi,rsi,rdx,rcx,rbx,rax,rip,cs,rflags,rsp,ss;
} regs64_t;


typedef struct process_struct
{
    void *stack_top;
    uint16_t pid;
    priority_t priority;
    struct process_struct *next;
    struct process_struct *previous;
} process_t;
typedef struct
{
    process_t *head, *current, *tail;
  int size;
} processlist_t;
uint16_t EmeraldProc_Task_create_process(void (*entrypoint)(), priority_t priority, int ring);
void EmeraldProc_Scheduler_schedule_task();
void EmeraldProc_Scheduler_give_cpu();
void EmeraldProc_Scheduler_init();
#endif
