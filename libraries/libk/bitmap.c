#include <libk/bitmap.h>
#include <libk/logging.h>
#include <memory/pmm.h>
#include <system/panic.h>

static void set(size_t index, bool value, Bitmap *self)
{
    if (index > self->size)
    {
        log(ERROR, "Trying to set bit outside of the bitmap!");

        panic("Trying to set bit outside of the bitmap!");

        return;
    }

    size_t bit = index % 8; /* 8 since the bitmap is uint8_t */
    size_t byte = index / 8;

    if (value)
    {
        self->buffer[byte] |= (1 << (bit));
    }
    else
    {
        self->buffer[byte] &= ~(1 << (bit));
    }
}

static bool get(size_t index, Bitmap *self)
{
    if (index > self->size)
    {
        log(ERROR, "Trying to get bit outside of the bitmap!");

        panic("Trying to get bit outside of the bitmap!");

        return false;
    }

    size_t bit = index % 8; /* 8 since the bitmap is uint8_t */
    size_t byte = index / 8;

    return (self->buffer[byte] & (1 << (bit)));
}

Bitmap _Bitmap(uint8_t *data, size_t size)
{
    module("Bitmap");

    log(INFO, "Creating bitmap %x which is %d bytes", (uint64_t)data, size / 8);

    memset(data, 0xff, size / 8);

    Bitmap new_bitmap;

    new_bitmap.size = size;
    new_bitmap.buffer = data;
    new_bitmap.last_free = 0;
    new_bitmap.set = set;
    new_bitmap.get = get;

    return new_bitmap;
}
