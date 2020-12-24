#include "vmm.h"
#include "../physical/pmm.h"
#include <debug-utilities/logger.h>
void EmeraldMem_VMM_create_pagemap(pagemap_t *map)
{
    uint64_t page = EmeraldMem_PMM_allocate_block(4096);

    uint64_t *pml4 = (uint64_t *)page;

    memset(pml4, 0, 4096);

    map->pml4 = pml4;
}
uint64_t *walk_to_page_and_map(uint64_t *current, uint16_t index)
{
    if (current[index] == 0)
    {
        current[index] = EmeraldMem_PMM_allocate_block(4096);
        current[index] |= 0b11;
    }

    return (uint64_t *)current[index];
}
uintptr_t lower_half(uintptr_t arg)
{
    /* offset is defined in link.ld */
    return arg - 0xffffffff80000000;
}
uintptr_t higher_half(uintptr_t arg)
{
    /* offset is defined in link.ld */
    return arg + 0xffffffff80000000;
}
void EmeraldMem_VMM_map_page(pagemap_t *page_map, uintptr_t physical_adress, uint64_t virtual_adress, uintptr_t flags)
{
    /* get physical adress */
    physical_adress = lower_half(virtual_adress);
    /* Paging levels */
    uint16_t level1 = virtual_adress >> 12;
    uint16_t level2 = virtual_adress >> 21;
    uint16_t level3 = virtual_adress >> 30;
    uint16_t level4 = virtual_adress >> 39;
    uint64_t *root = page_map->pml4;

    walk_to_page_and_map(root, level4);

    walk_to_page_and_map(root, level3);

    uint64_t *pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = physical_adress | flags;
}

void EmeraldMem_VMM_initialize()
{
    /* This function maps the first 4gb of ram */
    pagemap_t *page_map = page_map;

    EmeraldMem_VMM_create_pagemap(page_map);

    uint64_t *root = page_map->pml4;

    uintptr_t root_lower_half = lower_half(*root);

    /* Map the 4 gb */
    for (uint64_t i = 0; i < 0x100000000; i += 0x1000)
    {
        EmeraldMem_VMM_map_page(page_map, i, higher_half(i), 0b11);
    }
    /* Loads root into cr3 */
    asm volatile("mov %%cr3,%0" ::"r"(&root_lower_half)
                 : "memory");
    log(INFO, "Mapped Kernel");
}
void EmeraldMem_VMM_unmap_page(pagemap_t *page_map, uint64_t virtual_adress)
{
    uint16_t level1 = virtual_adress >> 12;
    uint16_t level2 = virtual_adress >> 21;
    uint16_t level3 = virtual_adress >> 30;
    uint16_t level4 = virtual_adress >> 39;

    uint64_t *root = page_map->pml4;
    walk_to_page_and_map(root, level4);
    walk_to_page_and_map(root, level3);
    uint64_t *pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = 0;
}
