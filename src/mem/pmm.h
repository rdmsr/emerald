#ifndef PHYSICAL_MEMORY_MANAGER
#define PHYSICAL_MEMORY_MANAGER
#include "../inc/stivale.h"

void EmeraldMem_PMM_init_pmm(stivale_info_t *info);
void EmeraldMem_PMM_pmm_set_page_used(uint64_t page);
void EmeraldMem_PMM_pmm_set_page_free(uint64_t page);
uint8_t EmeraldMem_PMM_is_page_used(uint64_t page);
uint64_t EmeraldMem_PMM_pmm_find_free_pages(uint64_t size);
void *EmeraldMem_PMM_pmm_alloc(uint64_t size);
void EmeraldMem_PMM_pmm_unalloc(void *addr, uint64_t size);
#endif
