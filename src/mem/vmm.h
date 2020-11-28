#ifndef VIRTUAL_MEMORY_MANAGER
#define VIRTUAL_MEMORY_MANAGER

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    //TODO: maybe implement lock
    uintptr_t* pml4;
}pagemap_t __attribute__((aligned(4096)));

bool vmm_create_pagemap(pagemap_t *map);
bool vmm_map_page(pagemap_t *page_map, uintptr_t virt, uintptr_t phys, uintptr_t flags);
bool vmm_unmap_page(pagemap_t *page_map, uintptr_t virt);

#endif