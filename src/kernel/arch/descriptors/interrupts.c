/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>
#include <arch/context.h>
#include <arch/descriptors/interrupts.h>
#include <arch/proc/sched.h>
#include <devices/apic.h>
#include <devices/pic.h>
#include <emerald/log.h>

volatile u64 ticks = 0;
volatile bool sched_init = false;

static const char *exceptions[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Aligment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"};

void toggle_sched_init()
{
    sched_init = !sched_init;
}

void interrupt_error_handler(Stack *stackframe)
{

    uint64_t cr0 = asm_read_cr0();
    uint64_t cr2 = asm_read_cr2();
    uint64_t cr3 = asm_read_cr3();

    log_error("");
    log_error(" _________        .---\"\"\"      \"\"\"---.");
    log_error(":______.-':      :  .--------------.  :");
    log_error("| ______  |      | :                : |");
    log_error("|:______B:|      | | Kernel Panic!  | |");
    log_error("|:______B:|      | |                | |");
    log_error("|:______B:|      | | {}     | |", exceptions[stackframe->intno]);
    log_error("|         |      | | int {x}        | |", stackframe->intno);
    log_error("|:_____:  |      | |                | |");
    log_error("|    ==   |      | :                : |");
    log_error("|       O |      :  '--------------'  :");
    log_error("|       o |      :'---...______...---'");
    log_error("|       o |-._.-i___/'             \\._");
    log_error("|'-.____o_|   '-.   '-...______...-'  `-._");
    log_error(":_________:   .____________________   `-.___.-.");
    log_error("             .'.eeeeeeeeeeeeeeeeee.'.      :___:");
    log_error("           .'.eeeeeeeeeeeeeeeeeeeeee.'.");
    log_error("          :____________________________:");

    log_error("*** Stacktrace ***");
    log_error("Exception number: {x}, error: {x}", stackframe->intno, stackframe->err);
    log_error("RAX={p} RBX={p} RCX={p} RDX={p}", stackframe->rax, stackframe->rbx, stackframe->rcx, stackframe->rdx);
    log_error("RSI={p} RDI={p} RBP={p} RSP={p}", stackframe->rsi, stackframe->rdi, stackframe->rbp, stackframe->rsp);
    log_error("R8= {p} R9= {p} R10={p} R11={p}", stackframe->r8, stackframe->r9, stackframe->r10, stackframe->r11);
    log_error("R12={p} R13={p} R14={p} R15={p}", stackframe->r12, stackframe->r13, stackframe->r14, stackframe->r15);
    log_error("CR0={p} CR2={p} CR3={p} RIP={p}", cr0, cr2, cr3, stackframe->rip);
}

uint64_t interrupts_handler(uint64_t rsp)
{
    Stack *stackframe = (Stack *)rsp;

    if (stackframe->intno < 32)
    {
        interrupt_error_handler(stackframe);

        while (1)
        {
            __asm__("cli");
            asm_hlt();
        }
    }

    if (stackframe->intno == 32)
    {
        ticks++;

        if (sched_init)
        {
            log("stack before: {p}", (u64)stackframe->rsp);
            sched_schedule(stackframe);

            log("stack after: {p}", (u64)stackframe->rsp);
        }
    }

    /* Syscalls:
        * rax: number
        * rbx Arg1
	* rcx Arg2
	* rdx Arg3
	* rsi Arg4
	*/
    
    if (stackframe->intno == 0x80)
    {
        if ((int)stackframe->rax == 1)
        {
            log("This is ground control to major Tom");
            log("You've really made the grade");
            log("And the papers want to know whose shirts you wear");
            log("Now it's time to leave the capsule if you dare");
        }
	
        else
        {

            log_syscall("{}", (char *)stackframe->rbx);
        }
    }

    apic_eoi();

    return rsp;
}

u64 get_ticks()
{
    return ticks;
}
