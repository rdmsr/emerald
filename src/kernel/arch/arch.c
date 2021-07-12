/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include <arch/arch.h>

static void serial_writer(char *string)
{
    com_write_string(COM1, string);
}

Writer *arch_debug_writer()
{

    static Writer serial;
    serial.write = serial_writer;
    return &serial;
}

void arch_initialize_descriptors(void)
{
    gdt_initialize();
    idt_initialize();
}

void arch_initialize_memory(struct stivale2_struct *bootinfo)
{
    pmm_initialize(bootinfo);
    vmm_initialize(bootinfo);
}
