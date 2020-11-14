#include <stdbool.h>
#include <stdint.h>
typedef struct {
  uint64_t* pml4;
} pagemap_t;
uint32_t page_directory[512] __attribute__((aligned(4096)));
uint32_t first_page_table[512] __attribute__((aligned(4096)));
void EmeraldMem_VMM_vmm_create_pagemap(pagemap_t *map);
void EmeraldMem_VMM_vmm_create_page_directory();
void EmeraldMem_VMM_vmm_create_first_pagemap();
bool EmeraldMem_VMM_vmm_map_page(pagemap_t *pagemap, uintptr_t virt, uintptr_t phys, uintptr_t flags);
bool EmeraldMem_VMM_vmm_unmap_page(pagemap_t *pagemap, uintptr_t virt);
