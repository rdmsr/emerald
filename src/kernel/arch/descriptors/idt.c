/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>
#include <arch/descriptors/idt.h>
#include <devices/pic.h>

extern void idt_flush(uintptr_t idt_ptr);

static IDTDescriptor idt[256];
static IDTPointer idtr;
extern uintptr_t __interrupt_vector[];

static IDTDescriptor idt_make_entry(uint64_t offset, uint8_t type)
{
    return (IDTDescriptor){
        .selector = 0x08,
        .offset_lo = offset & 0xFFFF,
        .offset_mid = (offset >> 16) & 0xFFFF,
        .offset_hi = (offset >> 32) & 0xFFFFFFFF,
        .ist = 0,
        .zero = 0,
        .type_attr = type};
}

void install_isr(void)
{
    //pic_remap();

    size_t i;
    for (i = 0; i < 3; i++)
    {
        idt[i] = idt_make_entry(__interrupt_vector[i], INTGATE);
    }

    idt[3] = idt_make_entry(__interrupt_vector[3], TRAPGATE);
    idt[4] = idt_make_entry(__interrupt_vector[4], TRAPGATE);

    size_t j;
    for (j = 5; j < 48; j++)
    {
        idt[j] = idt_make_entry(__interrupt_vector[j], INTGATE);
    }

    idt[128] = idt_make_entry(__interrupt_vector[j], 0xee);
}

void idt_initialize()
{
    idtr.size = (256 * sizeof(IDTDescriptor)) - 1;

    idtr.addr = (uint64_t)idt;

    log("Initializing ISRs...");
    install_isr();

    log("Loading IDT...");

    idt_flush((uintptr_t)&idtr);

    asm_sti();

    log("IDT initialized");
}
