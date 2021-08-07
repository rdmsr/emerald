#include <emerald/debug.h>
#include <emerald/ds/bitmap.h>

static uint8_t buf[20];

static Bitmap bitmap = {.size = 20, .data = buf};

TEST(bitmap_fill)
{
    bitmap_fill(&bitmap, 1);

    UNIT_ASSERT_EQUAL(bitmap_get(&bitmap, 0), 1);
}

TEST(bitmap_get)
{
    UNIT_ASSERT_EQUAL(bitmap_get(&bitmap, 0), 1);
}

TEST(bitmap_clear)
{
    bitmap_clear(&bitmap, 0);
    UNIT_ASSERT_EQUAL(bitmap_get(&bitmap, 0), 0);
}

TEST(bitmap_set)
{
    bitmap_set(&bitmap, 0);
    UNIT_ASSERT_EQUAL(bitmap_get(&bitmap, 0), 1);
}
