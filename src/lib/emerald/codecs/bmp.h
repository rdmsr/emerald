/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEMERALD_BMP_H
#define LIBEMERALD_BMP_H
#include <emerald/std.h>

typedef struct
{
    uint16_t bf_type;
    uint32_t bf_size;
    uint32_t reserved;
    uint32_t bf_offset;

    uint32_t bi_size;
    uint32_t bi_width;
    uint32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_cnt;
    uint32_t bi_compression;
    uint32_t bi_size_image;
    uint32_t bi_x_per_met;
    uint32_t bi_y_per_met;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;

} __attribute__((packed)) BitmapHeader;

typedef struct
{
    BitmapHeader hdr;
    uint32_t *image_buffer;
} BitmapImage;

void load_bmp(BitmapImage* image, BitmapHeader header);

#endif
