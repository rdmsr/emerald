#include "vbe.h"
#include <debug-utilities/logger.h>
#include <mem/virtual/vmm.h>
#include <stddef.h>
struct stivale2_struct_tag_framebuffer *fb_info;
color_t bg_color = {52, 84, 94};

uint32_t get_color(color_t *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}


void EmeraldDevices_VBE_draw_pixel(size_t x, size_t y, uint32_t color)
{
    size_t fb_i = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    fb[fb_i] = color;
    log(DEBUG, "Drawing pixel with color %d", color);
}
void vbe_clear_screen()
{
  
    color_t *color;
    color = &bg_color;
    EmeraldDevices_VBE_draw_pixel(1, 1, get_color(color));
    for (int i = 0; i < fb_info->framebuffer_width; i++)
    {
        for (int j = 0; j < fb_info->framebuffer_height; j++)
        {
            EmeraldDevices_VBE_draw_pixel(i, j, get_color(color));
        }
    }
}
void EmeraldDevices_VBE_init(struct stivale2_struct_tag_framebuffer *fb)
{
    size_t fb_size = fb->framebuffer_height * fb->framebuffer_pitch;

    pagemap_t *pagemap = pagemap;

    for (uint64_t i = fb->framebuffer_addr; i < fb->framebuffer_addr + fb_size; i += 4096)
    {
        EmeraldMem_VMM_map_page(pagemap, i, higher_half(i), 0b11);
    }
    log(DEBUG, "found a %dx%d display", fb->framebuffer_height, fb->framebuffer_height);
}
