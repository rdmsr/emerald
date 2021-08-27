/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include <arch/descriptors/gdt.h>

static GDTPointer gdtr;
static GDT gdt = {};
static uint32_t lock;

static TSS tss = {
    .reserved = 0,
    .rsp = {},
    .reserved0 = 0,
    .ist = {},
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .iopb_offset = 0,
};

TSSEntry make_tss_entry(uintptr_t tss)
{
    return (TSSEntry){
        .length = sizeof(TSSEntry),
        .base_low16 = tss & 0xffff,
        .base_mid8 = (tss >> 16) & 0xff,
        .flags1 = 0b10001001,
        .flags2 = 0,
        .base_high8 = (tss >> 24) & 0xff,
        .base_upper32 = tss >> 32,
        .reserved = 0,
    };
}

GDTDescriptor gdt_entry(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t flags)
{
    return (GDTDescriptor){
        .limit16 = (uint16_t)(limit & 0xffff),
        .base16 = (uint16_t)(base & 0xffff),
        .base23 = (uint8_t)(((base) >> 16) & 0xff),
        .flags = flags,
        .limit19 = (limit >> 16) & 0x0f,
        .granularity = granularity,
        .base31 = (uint8_t)((base >> 24) & 0xff),
    };
}

GDTDescriptor gdt_entry_simple(uint8_t flags, uint8_t granularity)
{
    return gdt_entry(0, 0, granularity, flags);
}

void gdt_initialize()
{
    gdtr.base = (uint64_t)&gdt;
    gdtr.limit = sizeof(GDT) - 1;

    gdt.entries[0] = gdt_entry(0, 0, 0, 0);
    gdt.entries[GDT_KERNEL_CODE] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_LONG_MODE_GRANULARITY);
    gdt.entries[GDT_KERNEL_DATA] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE, 0);

    gdt.entries[3] = gdt_entry(0, 0, 0, 0);
    
    gdt.entries[GDT_USER_DATA] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER, 0);
    gdt.entries[GDT_USER_CODE] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER, GDT_LONG_MODE_GRANULARITY);

    gdt.tss = make_tss_entry((uintptr_t)&tss);

    log("Loading GDT...");

    gdt_update((u64)&gdtr);

    log("GDT loaded");
}

void gdt_load_tss(TSS *tss)
{
    lock_acquire(&lock);
    
    gdt.tss = make_tss_entry((uintptr_t)tss);
    
    tss_update();

    lock_release(&lock);
}
