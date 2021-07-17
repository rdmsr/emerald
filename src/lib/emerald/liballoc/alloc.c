/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "alloc.h"
#include <emerald/debug.h>

void *liballoc_alloc(size_t size)
{
    size_t page_count = DIV_ROUNDUP(size, PAGE_SIZE);

    void *ptr = (char *)pmm_allocate_zero(page_count + 1);

    kassert(ptr != NULL);

    if (!ptr)
        return NULL;

    ptr += MEM_PHYS_OFFSET;

    return ptr;
}

int liballoc_lock(void) { return 0; }
int liballoc_unlock(void) { return 0; }

void liballoc_free_(void *ptr, int pages)
{
    pmm_free((void *)ptr - MEM_PHYS_OFFSET, pages + 1);
}
