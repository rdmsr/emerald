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

#include "pmm.h"
#include <debug-utilities/logger.h>
#include <stdint.h>
uint8_t *bitmap = (uint8_t *)(&end);
uint8_t *mem_start;
uint32_t total_blocks;
uint32_t bitmap_size;
void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}
void EmeraldMem_PMM_pmm_init(uint32_t mem_size)
{
    total_blocks = mem_size / BLOCK_SIZE;
    bitmap_size = total_blocks / BLOCKS_PER_BUCKET;
    if (bitmap_size * BLOCKS_PER_BUCKET < total_blocks)
        bitmap_size++;

    memset(bitmap, 0, bitmap_size);

    int mb = mem_size / (1024 * 1024);
    log(INFO, "Initializing PMM...\033[0;37m Bitmap adress = %d,Memory size = %d mb,Bitmap size = %d %s", bitmap, mb, bitmap_size, "\033[1;0mDone");
}

uint32_t EmeraldMem_PMM_allocate_block()
{
    uint32_t free_block = EmeraldMem_PMM_first_free_block();
    SETBIT(free_block);
    return free_block;
}

void EmeraldMem_PMM_free_block(uint32_t block_num)
{
    CLEARBIT(block_num);
}

uint32_t last_free_block = 0;
uint32_t EmeraldMem_PMM_first_free_block()
{
    uint32_t i;
    for (i = last_free_block; i < total_blocks; i++)
    {
        if (!ISSET(i))
        {
            last_free_block = i;
            return i;
        }
    }
    last_free_block = 0;
    return EmeraldMem_PMM_first_free_block();
}
