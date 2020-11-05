#include <stdbool.h>
#include <stdint.h>
typedef struct {
  uint64_t* pml4;
} pagemap_t;
bool *EmeraldMem_VMM_vmm_create_pagemap(pagemap_t *map);
bool EmeraldMem_VMM_vmm_map_page(pagemap_t *pagemap, uintptr_t virt, uintptr_t phys, uintptr_t flags);
bool EmeraldMem_VMM_vmm_unmap_page(pagemap_t *pagemap, uintptr_t virt);
uint64_t* EmeraldMem_VMM_vmm_walk_to_page_and_map(uint64_t* current, uint16_t index);
