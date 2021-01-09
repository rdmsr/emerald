/*
MIT License

Copyright (c) 2021 Abb1x

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
uintptr_t lower_half(uintptr_t arg);
uintptr_t higher_half(uintptr_t arg);
#endif
