#include "pmm.h"
#include "../devices/video/vga.h"
#include <stdint.h>

uint8_t * bitmap = (uint8_t*)(&end);
uint8_t * mem_start;
uint32_t total_blocks;
uint32_t bitmap_size;
void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
void EmeraldMem_PMM_pmm_init(uint32_t mem_size) {
    total_blocks = mem_size / BLOCK_SIZE;
    bitmap_size = total_blocks / BLOCKS_PER_BUCKET;
    if(bitmap_size * BLOCKS_PER_BUCKET < total_blocks)
        bitmap_size++;

    memset(bitmap, 0, bitmap_size);

    mem_start = (uint8_t*)BLOCK_ALIGN(((uint32_t)(bitmap + bitmap_size)));
}


uint32_t EmeraldMem_PMM_allocate_block() {
    uint32_t free_block = EmeraldMem_PMM_first_free_block();
    SETBIT(free_block);
    return free_block;
}

void EmeraldMem_PMM_free_block(uint32_t blk_num) {
    CLEARBIT(blk_num);
}


uint32_t EmeraldMem_PMM_first_free_block() {
    uint32_t i;
    for(i = 0; i < total_blocks; i++) {
        if(!ISSET(i))
            return i;
    }
    return (uint32_t) -1;
}
