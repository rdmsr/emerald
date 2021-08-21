#include "emerald/debug/unit.h"
#include <emerald/functional.h>
#include <emerald/log.h>

Either(int, String) either_func(int param)
{
    if (param == 666)
        return Left(int, String, -1);

    return Right(int, String, make_str("Hello world!"));
}

TEST(either_right)
{
    UNIT_ASSERT_STR_EQUAL(either_func(20).right, make_str("Hello world!"));
}

TEST(either_left)
{
    UNIT_ASSERT_EQUAL(either_func(666).left, -1);
}
