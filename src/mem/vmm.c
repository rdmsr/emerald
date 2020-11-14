#include "vmm.h"
#include "pmm.h"
void EmeraldMem_VMM_vmm_create_pagemap(pagemap_t *map){
  uint64_t page = EmeraldMem_PMM_allocate_block(sizeof(pagemap_t));
  if (map == 0) {
      return false;
  }
  uint64_t *pml4 = (uint64_t *)page;
  memset(pml4, 0, sizeof(pagemap_t));
  map->pml4 = pml4;
  return true;
}
