#ifndef TASK_H
#define TASK_H
#pragma once
#include <stdint.h>
#include <mem/virtual/vmm.h>
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

typedef struct process_struct
{
    int id;
    thread_t thread;
    pagemap_t* pagemap;
    char* name;
} process_t;

process_t EmeraldProc_Task_create_process(int id, uint8_t priority,uintptr_t physical_adress,thread_t thread, char *name);
void EmeraldProc_Scheduler_schedule_task();
#endif
