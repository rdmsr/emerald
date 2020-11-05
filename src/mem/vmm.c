#include "vmm.h"
#include "pmm.h"
bool *EmeraldMem_VMM_vmm_create_pagemap(pagemap_t *map){
  uint64_t page = EmeraldMem_PMM_allocate_block(sizeof(pagemap_t));
  if (map == 0) {
      return false;
  }
  uint64_t *pml4 = (uint64_t *)page;
  memset(pml4, 0, sizeof(pagemap_t));
  map->pml4 = pml4;
  return true;
}
//this function walks all the levels and then map them
uint64_t* EmeraldMem_VMM_vmm_walk_to_page_and_map(uint64_t* current, uint16_t index)
{
    if(current[index] == 0)
    {
        current[index] = EmeraldMem_PMM_allocate_block(sizeof(pagemap_t)) | 0b11; //bit 11 = available
    }

    return current[index];
}
