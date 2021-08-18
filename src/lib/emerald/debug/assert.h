/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_ASSERT_H
#define LIBEMERALD_ASSERT_H

#include <emerald/std.h>

#define assert_not_null(expr) (             \
    {                                       \
        if (expr == NULL)                   \
        {                                   \
            log_panic("{} is NULL", #expr); \
        }                                   \
    })

#define assert_truth(expr) (                 \
    {                                        \
        if ((expr) != true)                  \
        {                                    \
            log_panic("{} is false", #expr); \
        }                                    \
    })
#endif
