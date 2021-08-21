/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_EITHER_H
#define LIBEMERALD_EITHER_H

#include <emerald/std.h>
#include <emerald/str.h>

#define new_either(T1, T2) \
    typedef struct         \
    {                      \
        bool ok;           \
        union              \
        {                  \
            T1 left;       \
            T2 right;      \
        };                 \
    } __either_##T1##T2

new_either(int, int);
new_either(int, char);
new_either(String, int);
new_either(int, String);

#define Right(T1, T2, val) \
    (__either_##T1##T2)    \
    {                      \
        .ok = true,        \
        .right = (val),    \
    }

#define Left(T1, T2, error) \
    (__either_##T1##T2)     \
    {                       \
        .ok = false,        \
        .left = (error),    \
    }

#define Either$(expr) (                                         \
    {                                                           \
        auto __expr = (expr);                                   \
                                                                \
        if (!__expr.ok)                                         \
        {                                                       \
            log_panic("Either failed, left = {}", __expr.left); \
        }                                                       \
                                                                \
        __expr.right;                                           \
    })

#define Either(t, t2) __either_##t##t2
#endif
