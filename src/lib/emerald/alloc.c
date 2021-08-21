/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "alloc.h"
#include "liballoc/liballoc.h"
#include <emerald/debug.h>

#ifndef HOST
static uint32_t lock = 0;

void *liballoc_alloc(int size)
{
    size_t page_count = DIV_ROUNDUP(size, PAGE_SIZE);

    void *ptr = (char *)pmm_allocate_zero(page_count + 1);

    assert_not_null(ptr);

    if (!ptr)
        return NULL;

    ptr += MEM_PHYS_OFFSET;

    return ptr;
}

int liballoc_lock(void)
{
    lock_acquire(&lock);
    return 0;
}
int liballoc_unlock(void)
{
    lock_release(&lock);
    return 0;
}

int liballoc_free_(void *ptr, int pages)
{
    pmm_free((void *)ptr - MEM_PHYS_OFFSET, pages + 1);
    return 0;
}

void *alloc_malloc(size_t size)
{
    return liballoc_alloc(size);
}

void alloc_free(void *ptr)
{
    return liballoc_free(ptr);
}

void *alloc_realloc(void *p, size_t size)
{
    return liballoc_realloc(p, size);
}

void *alloc_malloc_end(size_t size)
{
    return alloc_malloc(size) + size;
}
#endif
