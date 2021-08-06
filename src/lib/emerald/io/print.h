/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_PRINT_H
#define LIBEMERALD_PRINT_H

#include <emerald/io/rw.h>

#define print(stream, fmt, ...) fmt_stream(stream, fmt, __VA_ARGS__)

#endif
