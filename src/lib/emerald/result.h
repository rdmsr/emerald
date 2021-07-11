/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_RESULT_H
#define LIBEMERALD_RESULT_H
#include <emerald/std.h>

#define Result(TError, TOk) \
    struct                  \
    {                       \
        bool success;       \
        union               \
        {                   \
            TError error;   \
            TOk ok;         \
        };                  \
    }

#define OK(T, value)     \
    (T)                  \
    {                    \
        .success = true, \
        .ok = (value),   \
    }

#define ERR(T, value)     \
    (T)                   \
    {                     \
        .success = false, \
        .error = (value)  \
    }

#define TRY(T, expr)                       \
    (                                      \
        {                                  \
            if (!expr.success)             \
            {                              \
                return ERR(T, expr.error); \
            }                              \
            expr.ok;                       \
        })

#endif
