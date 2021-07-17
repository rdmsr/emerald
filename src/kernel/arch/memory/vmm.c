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

void vmm_map_range(uint64_t *pagemap, uint64_t start, uint64_t end, uint64_t offset, uint64_t flags)
{

    uintptr_t i;

    for (i = start; i < end; i += PAGE_SIZE)
    {
        vmm_map_page(pagemap, i, i + offset, flags);
    }
}

void load_pagemap(uint64_t *pagemap)
{
    asm_write_cr3((uint64_t)pagemap);
}

void vmm_initialize(struct stivale2_struct *stivale2_struct)
{

    struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    kassert(memory_map != NULL);

    size_t i;
    uintptr_t p;

    kernel_pagemap = (uint64_t *)pmm_allocate_zero(1);

    vmm_map_range(kernel_pagemap, 0, 0x8000000, KERNEL_BASE, 0b11);
    vmm_map_range(kernel_pagemap, 0, 0x100000000, MEM_PHYS_OFFSET, 0b11);

    for (i = 0; i < memory_map->entries; i++)
    {
        if (memory_map->memmap[i].type == STIVALE2_MMAP_USABLE)
        {
            for (p = 0; p < memory_map->memmap[i].length; p += PAGE_SIZE)
            {
                vmm_map_page(kernel_pagemap, p, p + MEM_PHYS_OFFSET, 0b11);
            }
        }

        log("Mapped {i}/{i} of the memory map", i + 1, memory_map->entries);
    }
    
    log("Mapped memory map");

    load_pagemap(kernel_pagemap);

    log("initialized vmm");
}
