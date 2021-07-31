#include <emerald/debug.h>
#include <emerald/ds/vec.h>

vec_int_t vec;

TEST(vec_init)
{
    vec_init(&vec);

    UNIT_ASSERT_EQUAL(vec.data, 0);
}

TEST(vec_push)
{
    vec_push(&vec, 10);
    UNIT_ASSERT_EQUAL(vec.data[vec.length - 1], 10);
}

TEST(vec_pop)
{
    int prev_length = vec.length;
    vec_pop(&vec);
    UNIT_ASSERT(vec.length < prev_length);
}

TEST(vec_first)
{
    vec_push(&vec, 10);
    vec_push(&vec, 20);

    int a = vec_first(&vec);

    UNIT_ASSERT_EQUAL(a, 10);
}

TEST(vec_last)
{
    int a = vec_last(&vec);

    UNIT_ASSERT_EQUAL(a, 20);
}
