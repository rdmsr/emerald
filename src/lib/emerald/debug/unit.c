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

void test_add(struct test test)
{
    tests[test_count++] = test;
}

void __unit_assert(bool val, char *equality)
{
    if (!val)
    {
        log_fail("assertion failed: {a}", equality);
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

void test_run_all(void)
{
    log("====== Running {i} tests ======", test_count);

    for (size_t i = 0; i < test_count; i++)
    {
        auto test = tests[i];
        auto result = test_run(test);

        if (result == TEST_PASS)
        {
            log_success("SUCCESS - {s}", test.name);
        }

        if (result == TEST_FAIL)
        {
            log_fail("FAIL - {s}", test.name);
        }
    }

    log("====== Statistics: {i} successes | {i} fails ======",success_count, fail_count);
}
