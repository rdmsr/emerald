/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "vmm.h"
#include <emerald/debug.h>

static uint64_t *kernel_pagemap;

static uint64_t *get_next_level(uint64_t *table, size_t index)
{
    if (!(table[index] & 1))
    {
        table[index] = (uint64_t)pmm_allocate_zero(1);
        table[index] |= 0b11;
    }
    
    return (uint64_t *)((table[index] & ~(0x1ff)) + MEM_PHYS_OFFSET);
}

void vmm_map_page(uint64_t *pagemap, uintptr_t physical_address, uintptr_t virtual_address, uint64_t flags)
{
    size_t level4 = PML_ENTRY(virtual_address, 39);
    size_t level3 = PML_ENTRY(virtual_address, 30);
    size_t level2 = PML_ENTRY(virtual_address, 21);
    size_t level1 = PML_ENTRY(virtual_address, 12);

    uint64_t *pml3 = get_next_level(pagemap, level4);
    uint64_t *pml2 = get_next_level(pml3, level3);
    uint64_t *pml1 = get_next_level(pml2, level2);

    pml1[level1] = physical_address | flags;
}

void vmm_unmap_page(uint64_t *pagemap, uintptr_t virtual_address)
{
    size_t level4 = PML_ENTRY(virtual_address, 39);
    size_t level3 = PML_ENTRY(virtual_address, 30);
    size_t level2 = PML_ENTRY(virtual_address, 21);
    size_t level1 = PML_ENTRY(virtual_address, 12);

    uint64_t *pml3 = get_next_level(pagemap, level4);
    uint64_t *pml2 = get_next_level(pml3, level3);
    uint64_t *pml1 = get_next_level(pml2, level2);

    pml1[level1] = 0;
}

void load_pagemap(uint64_t *pagemap)
{
    __asm__ volatile("mov %0, %%cr3"
                     :
                     : "r"(pagemap));
}

void vmm_initialize()
{
    kernel_pagemap = (uint64_t *)pmm_allocate_zero(1);

    uintptr_t i, j;
    for (i = 0; i < 0x80000000; i += PAGE_SIZE)
    {
        vmm_map_page(kernel_pagemap, i, i + KERNEL_BASE, 0b11);
    }

    for (j = 0; j < 0x100000000; j += PAGE_SIZE)
    {
        vmm_map_page(kernel_pagemap, j, j + MEM_PHYS_OFFSET, 0b11);
    }

    load_pagemap(kernel_pagemap);

    log(INFO, "initialized vmm");
}
