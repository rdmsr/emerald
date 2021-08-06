/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_FMT_H
#define LIBEMERALD_FMT_H
#include <emerald/io.h>
#include <emerald/std.h>
#include <emerald/str/str.h>

enum FormatType
{
    FORMAT_DECIMAL,
    FORMAT_CHAR,
    FORMAT_STRING,
    FORMAT_BOOL
};

typedef struct
{
    enum FormatType type;

    union
    {
        int64_t decimal;
        char const *_string;
    };
} FormatValue;

typedef struct
{
    size_t count;
    FormatValue *values;
} FormatValues;

static inline FormatValue fmtvald(int64_t val)
{
    return (FormatValue){
        FORMAT_DECIMAL,
        {.decimal = val},
    };
}

static inline FormatValue fmtvals(char const *val)
{
    return (FormatValue){
        FORMAT_STRING,
        {._string = val},
    };
}

static inline FormatValue fmtvalstr(String val)
{
    return (FormatValue){
        FORMAT_STRING,
        {._string = val.buffer},
    };
}

// clang-format off
#define SELECT_VALUE(VALUE)                                                                        \
    _Generic(\
    (VALUE), \
    int64_t: fmtvald, \
    uint8_t: fmtvald, \
    uint16_t: fmtvald, \
    uint32_t: fmtvald, \
    size_t: fmtvald, \
    _Bool: fmtvald, \
    int: fmtvald, \
    char*: fmtvals, \
    const char*: fmtvals, \
    String: fmtvalstr)(VALUE),
// clang-format on

#define FORMAT_ARGS_(...) \
    (FormatValues) { 0, (FormatValue[]){}, }

#define FORMAT_ARGS_N(...)                                                             \
    (FormatValues)                                                                     \
    {                                                                                  \
        GET_ARG_COUNT(__VA_ARGS__), (FormatValue[]) { MAP(SELECT_VALUE, __VA_ARGS__) } \
    }

#define FORMAT_ARGS(...) \
    FORMAT_ARGS_##__VA_OPT__(N)(__VA_ARGS__)

#define fmt_buffer(buf, __format, ...) \
    __fmt_buffer(buf, __format, FORMAT_ARGS(__VA_ARGS__))

#define fmt_stream(stream, fmt, ...) __fmt_stream(stream, fmt, FORMAT_ARGS(__VA_ARGS__))

String __fmt_buffer(char *buffer, char *format, FormatValues values);
void __fmt_stream(Writer *stream, char *format, FormatValues values);

#endif
