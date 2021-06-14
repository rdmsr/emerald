/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <arch/asm.h>
#include <arch/descriptors/idt.h>

extern void isr_irq_slave(void);
extern void isr_irq_master(void);

static IDTDescriptor idt[256];
static IDTPointer idtr;

void pic_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0x0);
    outb(0xA1, 0x0);
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

    int i, j;
    for (i = 0x20; i < 0x28; i++)
    {
        idt[i] = idt_make_entry((uint64_t)&isr_irq_master);
    }
    for (j = 0x28; j < 0x2F; j++)
    {
        idt[j] = idt_make_entry((uint64_t)&isr_irq_slave);
    }
}

void idt_load(void)
{
    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
}

void idt_initialize()
{
    idtr.size = 256 * sizeof(IDTDescriptor);
    idtr.addr = (uint64_t)idt;

    log(INFO, "Initializing ISRs...");
    install_isr();
    
    log(INFO, "Loading IDT...");
    idt_load();

    sti();

    log(INFO, "IDT initialized");
}
