#ifndef PMM_H
#define PMM_H
#include <stddef.h>
#include <stdint.h>
#define BLOCK_SIZE 4096
#define BLOCKS_PER_BUCKET 8
#define SETBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i) ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t*)&bitmap[i / 32]))


#define BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000)
extern uint32_t end;

void EmeraldMem_PMM_pmm_init(uint32_t mem_size);
uint32_t EmeraldMem_PMM_allocate_block();
void EmeraldMem_PMM_free_block(uint32_t blk_num);
uint32_t EmeraldMem_PMM_first_free_block();
void* memset(void* bufptr, int value, size_t size);
#endif
