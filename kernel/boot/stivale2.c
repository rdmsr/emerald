#include "stivale2.h"
#include <main.h>
#include <stdint.h>

static uint8_t stack[4096] = {0};
struct stivale2_header_tag_framebuffer fb_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0},
    .framebuffer_height = 0,
    .framebuffer_width = 0};
struct stivale2_header_tag_smp smp_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = (uint64_t)&fb_request},
    .flags = 0};

__attribute__((section(".stivale2hdr"), used)) struct stivale2_header header2 = {
    .entry_point = (uint64_t)kmain,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uint64_t)&smp_request,

};
