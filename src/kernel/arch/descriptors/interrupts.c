/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>
#include <arch/descriptors/interrupts.h>
#include <emerald/log.h>

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

uint64_t interrupts_handler(uint64_t rsp)
{
    InterruptStackframe *stackframe = (InterruptStackframe *)rsp;

    if (stackframe->intno < 32)
    {
        uint64_t cr0 = asm_read_cr0();
        uint64_t cr2 = asm_read_cr2();
        uint64_t cr3 = asm_read_cr2();
        log(ERROR, "");
        log(ERROR, " _________        .---\"\"\"      \"\"\"---.");
        log(ERROR, ":______.-':      :  .--------------.  :");
        log(ERROR, "| ______  |      | :                : |");
        log(ERROR, "|:______B:|      | | Kernel Panic!  | |");
        log(ERROR, "|:______B:|      | |                | |");
        log(ERROR, "|:______B:|      | | {a}     | |", exceptions[stackframe->intno]);
        log(ERROR, "|         |      | | int {x}        | |", stackframe->intno);
        log(ERROR, "|:_____:  |      | |                | |");
        log(ERROR, "|    ==   |      | :                : |");
        log(ERROR, "|       O |      :  '--------------'  :");
        log(ERROR, "|       o |      :'---...______...---'");
        log(ERROR, "|       o |-._.-i___/'             \\._");
        log(ERROR, "|'-.____o_|   '-.   '-...______...-'  `-._");
        log(ERROR, ":_________:   .____________________   `-.___.-.");
        log(ERROR, "             .'.eeeeeeeeeeeeeeeeee.'.      :___:");
        log(ERROR, "           .'.eeeeeeeeeeeeeeeeeeeeee.'.");
        log(ERROR, "          :____________________________:");

        log(ERROR, "*** Stacktrace ***");
        log(ERROR, "Exception number: {x}", stackframe->intno);
        log(ERROR, "RAX={p} RBX={p} RCX={p} RDX={p}", stackframe->rax, stackframe->rbx, stackframe->rcx, stackframe->rdx);
        log(ERROR, "RSI={p} RDI={p} RBP={p} RSP={p}", stackframe->rsi, stackframe->rdi, stackframe->rbp, stackframe->rsp);
        log(ERROR, "R8= {p} R9= {p} R10={p} R11={p}", stackframe->r8, stackframe->r9, stackframe->r10, stackframe->r11);
        log(ERROR, "R12={p} R13={p} R14={p} R15={p}", stackframe->r12, stackframe->r13, stackframe->r14, stackframe->r15);
        log(ERROR, "CR0={p} CR2={p} CR3={p} RIP={p}", cr0, cr2, cr3, stackframe->rip);

        while (1)
        {
            __asm__("cli");
            __asm__("hlt");
        }
    }
    return rsp;
}
