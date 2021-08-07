/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "unit.h"
#include <emerald/log.h>

static size_t test_count = 0;
static size_t fail_count = 0;
static size_t success_count = 0;
static struct test tests[1024] = {0};
static int last_fail_line = 0;
static char *last_fail_file, *last_fail_test;

void test_add(struct test test)
{
    tests[test_count++] = test;
}

void __unit_assert(bool val, int line, char *file, char *equality)
{
    if (!val)
    {
        last_fail_line = line;
        last_fail_file = file;
        last_fail_test = equality;
        fail_count++;
    }
    else
    {
        success_count++;
    }

    return;
}

enum test_result test_run(struct test test)
{
    size_t previous_fail_count = fail_count;

    test.func();

    if (fail_count > previous_fail_count)
    {
        return TEST_FAIL;
    }

    return TEST_PASS;
}

int test_run_all(void)
{
    log("====== Running {} tests ======", test_count);

    for (size_t i = 0; i < test_count; i++)
    {
        auto test = tests[i];
        auto result = test_run(test);

        if (result == TEST_PASS)
        {
            log_pass("[ ✓ ] {}", test.name);
        }

        if (result == TEST_FAIL)
        {
            log_fail("[ ✗ ] {} at {}:{}", test.name, last_fail_file, last_fail_line);
        }
    }

    log("====== Statistics: {} successes | {} fails ======", success_count, fail_count);

    return fail_count > 0 ? -1 : 0;
}
