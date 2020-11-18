#include <stdbool.h>
#include <stdint.h>
typedef struct {
    uint64_t* pml4;
} pagemap_t;

void EmeraldMem_VMM_vmm_create_pagemap(pagemap_t pagemap);
void* get_physical_addr(void* virtualaddr);
void EmeraldMem_VMM_map_page(void* physaddr, void* virtualaddr, unsigned int flags);