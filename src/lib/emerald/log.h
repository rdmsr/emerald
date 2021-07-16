/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_LOG_H
#define LIBEMERALD_LOG_H

#include <arch/arch.h>
#include <emerald/io.h>
#include <emerald/macros.h>
#include <emerald/str.h>

typedef enum
{
    INFO,
    PANIC,
    ERROR,
    WARNING,
    DEBUG,
    TODO
} LogLevel;

void __log(LogLevel level, int line, char *file, char *format, ...);

#define log(format, ...) __log(INFO, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)
#define log_error(format, ...) __log(ERROR, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)
#define log_warning(format, ...) __log(WARNING, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)
#define log_todo(format, ...) __log(TODO, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)
#define log_panic(format, ...) __log(PANIC, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)
#define log_debug(format, ...) __log(DEBUG, __LINE__, __FILENAME__, format __VA_OPT__(,) __VA_ARGS__)

#endif
