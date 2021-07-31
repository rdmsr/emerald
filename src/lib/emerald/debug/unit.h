/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_UNIT_H
#define LIBEMERALD_UNIT_H
#include <emerald/std.h>
#include <emerald/str.h>

typedef void TestFunc(void);

enum test_result
{
    TEST_PASS,
    TEST_FAIL,
};

struct test
{
    String name;
    TestFunc *func;
};

void test_add(struct test test);

#define TEST_FUNCTION_NAME(name) test_##name

#define TEST(name)                                                                      \
    void TEST_FUNCTION_NAME(name)(void);                                                \
    __attribute__((constructor)) static inline void test_register_##name##_detail(void) \
    {                                                                                   \
        test_add((struct test){                                                         \
            make_str(#name),                                                            \
            TEST_FUNCTION_NAME(name),                                                   \
        });                                                                             \
    }                                                                                   \
    void TEST_FUNCTION_NAME(name)(void)

#define unit_assert(val) __unit_assert(val, #val)

void __unit_assert(bool val, char *equality);

void test_run_all(void);

#endif
