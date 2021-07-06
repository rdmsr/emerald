/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include <emerald/str/str.h>

size_t cstrlen(char *str)
{
    size_t i;

    for (i = 0; str[i] != '\0'; i++)
        ;
    return i;
}

String make_str(char *str)
{
    return (String){str, cstrlen(str)};
}

String str_concat(String src, String dest)
{
    size_t i;

    for (i = 0; i < src.size && src.buffer[i] != '\0'; i++)
    {
        dest.buffer[dest.size + i] = src.buffer[i];
    }

    dest.buffer[dest.size + i] = '\0';

    return dest;
}

String str_convert(unsigned int num, int base)
{
    static char representation[] = "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = representation[num % base];
        num /= base;
    } while (num != 0);
    return make_str(ptr);
}

bool str_ncmp(String str, String str2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (str.buffer[i] != str2.buffer[i])
        {
            if (str.buffer[1] < str2.buffer[1])
            {
                return false;
            }
            else
            {
                return false;
            }
        }

        if (str.buffer[i] == '\0')
        {
            return true;
        }
    }

    return true;
}
