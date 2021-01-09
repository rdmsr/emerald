/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* Modified version of @szhou42's PMM on github*/

#ifndef PMM_H
#define PMM_H
#include <stddef.h>
#include <stdint.h>

#define BLOCK_SIZE 4096
#define BLOCKS_PER_BUCKET 8
#define SETBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i) ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t *)&bitmap[i / 32]))

#define BLOCK_ALIGN(addr) (((addr)&0xFFFFF000) + 0x1000)
extern uint32_t end;

void EmeraldMem_PMM_pmm_init(uint32_t mem_size);
uint32_t EmeraldMem_PMM_allocate_block();
void EmeraldMem_PMM_free_block(uint32_t blk_num);
uint32_t EmeraldMem_PMM_first_free_block();
void *memset(void *bufptr, int value, size_t size);
#endif
