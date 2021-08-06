#ifndef LIBEMERALD_STR_H
#define LIBEMERALD_STR_H
#include <emerald/std.h>

typedef struct
{
    char *buffer;
    size_t size;
} String;

#define string_fixed(length) \
    struct                   \
    {                        \
        char buffer[length]; \
        size_t size;         \
    }

// clang-format off
#define make_str(x) _Generic((x),                       \
                             int64_t: make_str_i,							\
                             unsigned int: make_str_x,  \
			     unsigned long: make_str_x,	\
                             unsigned char: make_str_x, \
                             char*: make_str_cstr, \
                             char const*: make_str_cstr, \
			     char: make_str_char)(x)
// clang-format on

typedef string_fixed(8) StringFixed8;
typedef string_fixed(16) StringFixed16;
typedef string_fixed(32) StringFixed32;
typedef string_fixed(64) StringFixed64;
typedef string_fixed(128) StringFixed128;

size_t cstrlen(char *str);
String str_concat(String str, String str2);
String str_convert(uint16_t base, int64_t num);
bool str_ncmp(String str, String str2, int n);
bool str_cmp(String str, String str2);

String make_str_cstr(char const* str);
String make_str_i(int i);
String make_str_x(unsigned int hex);
String make_str_str(String str);
String make_str_char(char c);

char *itocstr(int64_t value, char *str, uint16_t base);
int cstrtoi(char *str);

#endif
