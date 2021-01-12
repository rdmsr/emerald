#include <mem/virtual/vmm.h>
#include <mem/physical/pmm.h>
#include "alloc.h"

void *kmalloc(size_t size)
{
    uint64_t size_data = (uint64_t) EmeraldMem_PMM_allocate_block(size + 0x2000) + 0xffffffff80000000;

    *(uint64_t *) size_data = size + 0x2000;
    *(uint64_t *) (size_data + 8) = 0xf100f333f100f333;


    return (void *) (size_data + 0x1000);
}

void *kcalloc(size_t size)
{
    void *buffer = kmalloc(size);
    memset((uint8_t *) buffer, 0, size);
    
    return buffer;
}
