#ifndef VIRTUAL_MEMORY_MANAGER
#define VIRTUAL_MEMORY_MANAGER

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uintptr_t *pml4;
} pagemap_t __attribute__((aligned(4096)));

void EmeraldMem_VMM_map_page(pagemap_t *page_map, uintptr_t physical_adress, uint64_t virtual_adress, uintptr_t flags);
void EmeraldMem_VMM_create_pagemap(pagemap_t *map);
void EmeraldMem_VMM_unmap_page(pagemap_t *page_map, uint64_t virtual_adress);
void EmeraldMem_VMM_initialize();
void EmeraldMem_set_bit(uint8_t* num, uint8_t bit, uint8_t state);
#endif
