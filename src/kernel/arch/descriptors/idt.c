/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>
#include <arch/descriptors/idt.h>

extern void idt_flush(uintptr_t idt_ptr);

static IDTDescriptor idt[256];
static IDTPointer idtr;
extern uintptr_t __interrupt_vector[];

void pic_remap(void)
{
    asm_outb(0x20, 0x11);
    asm_outb(0xA0, 0x11);

    asm_outb(0x21, 0x20);
    asm_outb(0xA1, 0x28);

    asm_outb(0x21, 0x04);
    asm_outb(0xA1, 0x02);

    asm_outb(0x21, 0x01);
    asm_outb(0xA1, 0x01);

    asm_outb(0x21, 0x0);
    asm_outb(0xA1, 0x0);
    log(INFO, "PIC remapped");
}

static IDTDescriptor idt_make_entry(uint64_t offset)
{
    return (IDTDescriptor){
        .selector = 0x08,
        .offset_lo = offset & 0xFFFF,
        .offset_mid = (offset >> 16) & 0xFFFF,
        .offset_hi = (offset >> 32) & 0xFFFFFFFF,
        .ist = 0,
        .zero = 0,
        .type_attr = 0x8e};
}


void install_isr(void)
{
    pic_remap();

    int j;
    for (j = 0; j < 48; j++)
    {
        idt[j] = idt_make_entry(__interrupt_vector[j]);
    }
}

void idt_initialize()
{
    idtr.size = 256 * sizeof(IDTDescriptor);
    idtr.addr = (uint64_t)idt;

    log(INFO, "Initializing ISRs...");
    install_isr();

    log(INFO, "Loading IDT...");

    idt_flush((uintptr_t)&idtr);

    asm_sti();

    log(INFO, "IDT initialized");
}
