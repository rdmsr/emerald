#ifndef BITMAP_H
#define BITMAP_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct bitmap_struct
{

    /* Functions */
    void (*set)(size_t, bool, struct bitmap_struct *); /* It's a bool because bits are only 0 or 1 */
    bool (*get)(size_t, struct bitmap_struct *);

    size_t (*find_free)(size_t, struct bitmap_struct *);
    size_t (*allocate)(size_t, struct bitmap_struct *);
    size_t (*set_free)(size_t, size_t, struct bitmap_struct *);
    size_t (*set_used)(size_t, size_t, struct bitmap_struct *);

    /* Variables */
    size_t size, last_free;
    uint8_t *buffer;

} Bitmap;

Bitmap _Bitmap(uint8_t *data, size_t size);

#endif
