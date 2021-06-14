/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include <arch/descriptors/gdt.h>

static GDTDescriptor gdt[3];
static GDTPointer gdtr;

void gdt_load()
{
    __asm__ volatile(
        "lgdt %0\n"
        "mov %%rsp, %%rax\n"
        "push $0x10\n"
        "push %%rax\n"
        " pushf\n"
        " push $0x08\n"
        " push $1f\n %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        :
        : "m" (gdtr)
        : "rax", "memory");
	
}

void gdt_initialize()
{
    gdtr.base = (uint64_t)&gdt;
    gdtr.limit = sizeof(gdt) - 1;

    gdt[0] = (GDTDescriptor){.access = 0, .granularity = 0};
    gdt[1] = (GDTDescriptor){.access = 0b10011010, .granularity = 0b00100000};
    gdt[2] = (GDTDescriptor){.access = 0b10010010, .granularity = 0};

    log(INFO, "Loading GDT...");
    gdt_load();
    log(INFO, "GDT loaded");
}
