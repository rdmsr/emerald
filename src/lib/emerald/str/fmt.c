/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "emerald/mem.h"
#include <emerald/ds/bitmap.h>
#include <emerald/str/fmt.h>

#define make_integer(base, type) \
    type i = va_arg(args, type); \
    if (i < (type)ZERO)          \
    {                            \
        i = -i;                  \
        buffer[position] = '-';  \
        position++;              \
    }                            \
    char ibuf[32] = {0};         \
    itocstr(i, ibuf, base);         \
    String s = make_str(ibuf);   \
    position += s.size;

void fmt_buffer(char *buffer, char *string, va_list args)
{
    Scanner scan = {.buffer = string, .size = cstrlen(string), .head = 0};

    unsigned int ZERO = 0;
    int position = 0;

    char pad_buffer[32] = {0};

    size_t pad;

    while (!scan_ended(&scan))
    {

        if (scan_current(&scan) == '{')
        {
            scan_forward(&scan);

            switch (scan_current(&scan))
            {
            case 'a':
            {
                char *s = va_arg(args, char *);
                str_concat(make_str(s), make_str(buffer));
                position += cstrlen(s);
                break;
            }

            case 's':
            {
                String s = va_arg(args, String);
                str_concat(s, make_str(buffer));
                position += cstrlen(s.buffer);
                break;
            }

            case 'i':
            {
                make_integer(10, int);
                str_concat(s, make_str(buffer));
                break;
            }

            case 'c':
            {
                char c = va_arg(args, int);
                buffer[position] = c;
                position++;
                break;
            }

            case 'm':
            {
                uintptr_t i = va_arg(args, uintptr_t);
                char ibuf[32] = {0};
                itocstr(i / 1024 / 1024, ibuf, 10);
                String s = make_str(ibuf);
                str_concat(s, make_str(buffer));
                position += s.size;
                break;
            }

            case 'x':
            {

                make_integer(16, uintptr_t);
                str_concat(make_str("0x"), make_str(buffer));
                position += 2;
                str_concat(s, make_str(buffer));
                break;
            }

            case 'b':
            {
                bool b = va_arg(args, int);
                str_concat(make_str(b ? "true" : "false"), make_str(buffer));
                position += b ? 4 : 5;
                break;
            }

            case 'p':
            {
                make_integer(16, uintptr_t);
                pad = 16;
                if (s.size < pad)
                {
                    mem_set(pad_buffer, '0', pad - s.size);
                    str_concat(make_str(pad_buffer), make_str(buffer));
                }

                str_concat(s, make_str(buffer));
                position += pad - s.size;

                break;
            }

            default:
                buffer[position] = '{';
                position++;
                break;
            }

            scan_forward(&scan);
        }

        else
        {
            buffer[position] = scan_current(&scan);
            position++;
        }

        scan_forward(&scan);
        scan_skip_c(&scan, '}');
    }
}

String fmt_str(char* buf, char *string, ...)
{
    va_list args;
    va_start(args, string);
    
    fmt_buffer(buf, string, args);

    va_end(args);

    return make_str(buf);
}
