/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_RW_H
#define LIBEMERALD_RW_H
#include <emerald/std.h>

typedef void Write(char *data);

typedef struct
{
    Write *write;
} Writer;

#define io_write(writer, data) \
    ((writer)->write((data)))

#endif
