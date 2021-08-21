#include "emerald/debug/unit.h"
#include "emerald/str/fmt.h"
#include <emerald/debug.h>
#include <emerald/mem.h>
#include <emerald/str.h>
#include <emerald/log.h>

static char buf[1024] = {0};

TEST(fmt_int)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{}", 10), make_str("10"));
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_bool)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{b}", true), make_str("true"));

    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_cstr)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{}", "hello"), make_str("hello"));
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_hex)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{x}", 0xfff), make_str("0xfff"));
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_char)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{c}", 'c'), make_str("c"));
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_str)
{
    UNIT_ASSERT_STR_EQUAL(fmt_buffer(buf, "{}", make_str("hello")), make_str("hello"));
    mem_set(buf, 0, sizeof(buf));
}


