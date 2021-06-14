/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_LOG_H
#define LIBEMERALD_LOG_H

#include <arch/arch.h>
#include <emerald/io.h>

typedef enum
{
    INFO,
    PANIC,
    ERROR,
    WARNING,
    DEBUG,
    TODO
} LogLevel;

void __log(LogLevel level, int line, char* file, char* format, ...);

#define log(level, format, ...) __log(level, __LINE__, __FILE__, format __VA_OPT__(,) __VA_ARGS__)

#endif
