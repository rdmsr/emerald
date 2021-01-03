#ifndef TASK_H
#define TASK_H
#pragma once
#include <stdint.h>
typedef struct
{
  uint64_t rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r8,r9,r10,r11,r12,r13,r14,r15;
} regs64_t;

typedef struct
{
  uint8_t priority;
  uint32_t time_slice;
  regs64_t registers;
} thread_t;

typedef struct
{
    uint64_t virtual_adress;
    int id;
    thread_t thread;
    char name[];
} process_t;

void EmeraldProc_timer_init();
void EmeraldProc_change_priority(uint8_t priority);

#endif
