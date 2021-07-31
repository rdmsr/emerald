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

/* These two functions were taken from https://github.com/not-microsowoft/experimental, licensed under GPL 3

Copyright (C) 2021 keyboard-slayer
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

*/

void strrev(char *s)
{
    char tmp;
    size_t end = cstrlen(s) - 1;
    size_t start = 0;

    while (start < end)
    {
        tmp = s[start];
        s[start++] = s[end];
        s[end--] = tmp;
    }
}


char *itoa(int64_t value, char *str, uint16_t base)
{
    size_t index = 0;
    int8_t digit;

    if (value < 0 && base != 10)
    {
        int64_t left;
        int64_t right;
        size_t i;

        char left_str[32];
        char right_str[32];

        right = value & 0xffffffff;
        left = (value & 0xffffffff00000000) >> 32;

        itoa(left, left_str, base);
        itoa(right, right_str, base);

        for (i = 0; i < cstrlen(left_str); i++)
        {
            str[i] = left_str[i];
        }

        for (i = 0; i < cstrlen(right_str); i++)
        {
            str[i + cstrlen(left_str)] = right_str[i];
        }

        return str;
    }

    else if (value < 0)
    {
        str[index++] = '-';
        value *= 1;
    }

    do
    {
        digit = value % base;

        if (digit < 0xa)
        {
            str[index++] = digit + '0';
        }
        else
        {
            str[index++] = (digit - 0xa) + 'a';
        }

        value /= base;
    } while (value);

    str[index] = 0;

    strrev(str);

    return str;
}

int atoi(char *str)
{
    int res = 0;

    int i;
    for (i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}
