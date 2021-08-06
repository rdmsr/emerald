/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_LOG_H
#define LIBEMERALD_LOG_H

#include <emerald/io.h>
#include <emerald/macros.h>
#include <emerald/str.h>

#ifndef HOST
#    include <arch/arch.h>
#else
#    include <stdio.h>
static inline void host_writer(char *string)
{
    printf("%s", string);
}
static inline Writer *arch_debug_writer()
{
    static Writer stdio;
    stdio.write = host_writer;
    return &stdio;
}
#endif
typedef enum
{
    INFO,
    PASS,
    FAIL,
    PANIC,
    ERROR,
    WARNING,
    DEBUG,
    TODO
} LogLevel;

void __log(LogLevel level, int line, char *file, char *format, FormatValues vals);
#define log(format, ...) __log(INFO, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_error(format, ...) __log(ERROR, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_warning(format, ...) __log(WARNING, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_todo(format, ...) __log(TODO, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_panic(format, ...) __log(PANIC, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_debug(format, ...) __log(DEBUG, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_fail(format, ...) __log(FAIL, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))
#define log_pass(format, ...) __log(PASS, __LINE__, __FILENAME__, format, FORMAT_ARGS(__VA_ARGS__))

#endif
