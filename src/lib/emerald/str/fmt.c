/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "emerald/io/scan.h"
#include "emerald/mem.h"
#include <emerald/ds/bitmap.h>
#include <emerald/log.h>
#include <emerald/str/fmt.h>

int check_digit(char c)
{
    if ((c >= '0') && (c <= '9'))
        return 1;
    return 0;
}

String __fmt_buffer(char *buffer, char *format, FormatValues values)
{
    Scanner scan = {.buffer = format, .size = cstrlen(format), .head = 0};

    size_t current_value = 0;

    char char_buf[2] = {0};

    bool is_digit = false;

    size_t prev_value = 0;

    while (!scan_ended(&scan))
    {
        char specifier = 0;

        if (scan_current(&scan) == '{')
        {
            while (scan_current(&scan) != '}')
            {
                scan_forward(&scan);

                if (scan_current(&scan) != '}')
                {
                    specifier = scan_current(&scan);
                }
            }

            is_digit = check_digit(specifier);

            if (is_digit)
            {
                prev_value = current_value;
                current_value = specifier - '0';
            }

            if (current_value < values.count)
            {
                switch (values.values[current_value].type)
                {

                case FORMAT_DECIMAL:
                {

                    char ibuffer[64] = {0};
                    char pad_buffer[32] = {0};

                    int base = 10;
                    size_t pad = 0;

                    bool is_bool = false;
                    bool is_char = false;

                    int64_t nbr = values.values[current_value].decimal;

                    switch (specifier)
                    {

                    case 'p':
                    {
                        base = 16;
                        pad = 16;
                        break;
                    }

                    case 'x':
                    {
                        base = 16;
                        pad = 0;
                        break;
                    }

                    case 'b':
                    {
                        is_bool = true;

                        break;
                    }

                    case 'c':
                    {
                        is_char = true;
                        break;
                    }

                    default:
                    {
                        base = 10;
                        pad = 0;
                        break;
                    }
                    }

                    if (!is_bool && !is_char)
                    {
                        itocstr(nbr, ibuffer, base);

                        if (base == 16)
                        {
                            str_concat(make_str("0x"), make_str(buffer));
                        }

                        if (cstrlen(ibuffer) < pad)
                        {
                            mem_set(pad_buffer, '0', pad - cstrlen(ibuffer));
                            str_concat(make_str(pad_buffer), make_str(buffer));
                        }

                        str_concat(make_str(ibuffer), make_str(buffer));
                    }

                    else if (is_bool)
                    {
                        str_concat(make_str(nbr ? "true" : "false"), make_str(buffer));
                    }

                    else
                    {
                        char_buf[0] = (char)values.values[current_value].decimal;
                        str_concat(make_str(char_buf), make_str(buffer));
                    }

                    break;
                }

                case FORMAT_STRING:
                {
                    str_concat(make_str(values.values[current_value]._string), make_str(buffer));
                    break;
                }

                default:
                    break;
                }
            }

            if (!is_digit)
            {
                current_value++;
            }

            else
            {
                current_value = prev_value + 1;
            }
        }

        else
        {
            char_buf[0] = scan_current(&scan);
            str_concat(make_str(char_buf), make_str(buffer));
        }

        scan_forward(&scan);
    }
    str_concat(make_str("\0"), make_str(buffer));

    return make_str(buffer);
}

void __fmt_stream(Writer *stream, char *format, FormatValues values)
{
    char buf[1024] = {0};

    __fmt_buffer(buf, format, values);

    io_write(stream, buf);

    mem_set(buf, 0, cstrlen(buf));
}
