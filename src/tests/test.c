#include <emerald/debug.h>
#include <emerald/log.h>
#include <emerald/mem.h>
#include <emerald/str.h>

static char buf[1024] = {0};

TEST(fmt_int)
{
    unit_assert(str_cmp(fmt_str(buf, "{i}", 10), make_str("10")) == true);

    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_bool)
{
    unit_assert(str_cmp(fmt_str(buf, "{b}", true), make_str("true")) == true);
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_str)
{
    unit_assert(str_cmp(fmt_str(buf, "{a}", "hello"), make_str("hello")) == true);
    mem_set(buf, 0, sizeof(buf));
}

TEST(fmt_hex)
{
    unit_assert(str_cmp(fmt_str(buf, "{x}", 0xfff), make_str("0xfff")) == true);
    mem_set(buf, 0, sizeof(buf));
}
int main()
{
    test_run_all();
    return 0;
}
