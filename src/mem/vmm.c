#include "vmm.h"
#include "pmm.h"

void EmeraldMem_VMM_create_pagemap(pagemap_t *map)
{
  uint64_t page = EmeraldMem_PMM_allocate_block(4096);
  
  uint64_t* pml4 = (uint64_t *)page;
  
  memset(pml4,0,4096);
  
  map->pml4 = pml4;
}
uint64_t* walk_to_page_and_map(uint64_t* current, uint16_t index)
{
    if(current[index] == 0)
    {
        current[index] = EmeraldMem_PMM_allocate_block(4096);
        current[index] |= 0b11; 
    }

    return (uint64_t *)current[index];
}   
void EmeraldMem_VMM_map_page(pagemap_t *page_map, uint64_t virtual_adress,uintptr_t flags)
{
  uintptr_t physical_adress = virtual_adress - 0xffffffff80000000;
  uint16_t level1 = virtual_adress>>12;
  uint16_t level2 = virtual_adress>>21;
  uint16_t level3 = virtual_adress>>30;
  uint16_t level4 = virtual_adress>>39;

  uint64_t* root = page_map->pml4;
  walk_to_page_and_map(root, level4);
  walk_to_page_and_map(root, level3);
  uint64_t* pml1 = walk_to_page_and_map(root, level2);
  pml1[level1] = physical_adress | flags;
  
  asm volatile ("mov %0, %%cr3" :: "r"(&root):"memory");
}

void EmeraldMem_VMM_unmap_page(pagemap_t *page_map, uint64_t virtual_adress)
{
  uint16_t level1 = virtual_adress>>12;
  uint16_t level2 = virtual_adress>>21;
  uint16_t level3 = virtual_adress>>30;
  uint16_t level4 = virtual_adress>>39;

  uint64_t* root = page_map->pml4;
  walk_to_page_and_map(root, level4);
  walk_to_page_and_map(root, level3);
  uint64_t* pml1 = walk_to_page_and_map(root, level2);

  pml1[level1] = 0;
}
