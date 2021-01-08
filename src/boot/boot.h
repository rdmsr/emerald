#ifndef BOOT_H
#define BOOT_H
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <inc/stivale2.h>
typedef struct
{
  size_t memory_usable;
  size_t total_memory;
  uint64_t framebuffer_addr;
  uint16_t framebuffer_width;
  uint16_t framebuffer_height;
  uint16_t framebuffer_pitch;
  uint16_t framebuffer_bpp;
} boot_info;

boot_info* EmeraldBoot_Stivale_get_boot_info(struct stivale2_struct *info);
#endif
