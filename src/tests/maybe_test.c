#include "emerald/debug/unit.h"
#include <emerald/functional.h>
#include <emerald/log.h>

Maybe(String) maybe_func(int param)
{
    if (param == 666)
        return Just(String, make_str("I like doom too!"));

    return Nothing(String);
}

TEST(maybe_just)
{
    UNIT_ASSERT_STR_EQUAL(maybe_func(666).just, make_str("I like doom too!"));
}

TEST(maybe_nothing)
{
    UNIT_ASSERT_EQUAL(maybe_func(665).ok, false);
}
