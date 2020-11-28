#include "vmm.h"
#include "pmm.h"
//This is me doing CPOP: Copy-Paste oriented programming, credits to @Kime78 on github
bool vmm_create_pagemap(pagemap_t *map)
{
    uint64_t page = EmeraldMem_PMM_allocate_block(4096);
    if (page == 0)
    {
        return false;
    }
    uint64_t *pml4 = (uint64_t *)page;
    memset(pml4, 0, 4096);
    map->pml4 = pml4;
    return true;
}

uint64_t* virtual_mem;

uint64_t* walk_to_page_and_map(uint64_t* current, uint16_t index)
{
    if(current[index] == 0)
    {
        current[index] = EmeraldMem_PMM_allocate_block(4096);
        current[index] |= 0b11; //bit available
    }

    return current[index] & ~(0b111111111111ULL);
}   

bool vmm_map_page(pagemap_t *page_map, uintptr_t virt, uintptr_t phys, uintptr_t flags)
{
    uint16_t offset = virt & 0b111111111111;    
    uint16_t level1 = (virt >> 12) & 0b111111111;
    uint16_t level2 = (virt >> 21) & 0b111111111;
    uint16_t level3 = (virt >> 30) & 0b111111111;
    uint16_t level4 = (virt >> 39) & 0b111111111;

    uint64_t* root = page_map->pml4;
    
    uint64_t* pml3 = walk_to_page_and_map(root, level4);
    uint64_t* pml2 = walk_to_page_and_map(root, level3);
    uint64_t* pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = phys | flags;

    uint64_t x = pml1[level1];

    //asm volatile ("mov %0, %%cr3" :: "r"(&root):"memory");
    return (x != 0);
}

bool vmm_unmap_page(pagemap_t *page_map, uintptr_t virt)
{
    uint16_t offset = virt & 0b111111111111;    
    uint16_t level1 = (virt >> 12) & 0b111111111;
    uint16_t level2 = (virt >> 21) & 0b111111111;
    uint16_t level3 = (virt >> 30) & 0b111111111;
    uint16_t level4 = (virt >> 39) & 0b111111111;

    uint64_t* root = page_map->pml4;
    uint64_t* pml3 = walk_to_page_and_map(root, level4);
    uint64_t* pml2 = walk_to_page_and_map(root, level3);
    uint64_t* pml1 = walk_to_page_and_map(root, level2);

    pml1[level1] = 0;
    return true;
}