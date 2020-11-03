#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../inc/stivale.h"
#include "../devices/video/vga.h"
bool* bitmap;
uint64_t memory_pages;

void* EmeraldMem_PMM_memoryset(void* bufptr, int value, size_t size){
unsigned char* buf = (unsigned char*) bufptr;
for(size_t i = 0; i < size; i++){

  buf[i] = (unsigned char) value;
}
return bufptr;
}
void EmeraldMem_PMM_init_pmm(stivale_info_t *info){
  mmap_entry_t *mmap = (void *) info->memory_map_addr;
  uint64_t memory_bytes = (mmap[info->memory_map_entries - 1].addr + mmap[info->memory_map_entries - 1].len);
  memory_pages = (memory_bytes + 0x1000 - 1) / 0x1000;
  uint64_t bitmap_bytes = (memory_pages + 8 - 1) / 8;
  bitmap = (void *) 0;
  for(uint64_t i = 1; i< info->memory_map_entries; i++){
    if(mmap[i].type == STIVALE_MEMORY_AVAILABLE){
      if (mmap[i].len >= bitmap_bytes){
        bitmap = (void *) (mmap[i].addr + 0xFFFF800000000000);
        break;

      }
    }

  }
  if (!bitmap) {
    kprint("[FAILED] Could not find address to put bitmap! Halting.",0x7);
    while (1) {
        asm volatile("hlt");
    }
}
  else{

    kprint_load("Bitmap",false);
  }
  for (uint64_t i = 1; i < info->memory_map_entries; i++) {
    if(mmap[i].type == STIVALE_MEMORY_AVAILABLE){
        if (mmap[i].addr == (uint64_t) bitmap) {
          uint64_t bitmap_end_byte = (uint64_t) bitmap + bitmap_bytes;
          uint64_t bitmap_end_page = ((bitmap_end_byte + 0x1000 - 1) / 0x1000) * 0x1000;
          uint64_t entry_end_page = (mmap[i].addr + mmap[i].len) / 0x1000;
          for (uint64_t page = bitmap_end_page; page < entry_end_page; page++) {
          EmeraldMem_PMM_pmm_set_page_free(page);
          }
        }
        else {
              uint64_t page = mmap[i].addr / 0x1000;
              uint64_t count = mmap[i].len / 0x1000;
              for (uint64_t j = 0; j < count; j++) {
                    EmeraldMem_PMM_pmm_set_page_free(page + j);
        }
    }
  }
}
kprint_load("PMM",false);
}

void EmeraldMem_PMM_pmm_set_page_used(uint64_t page){
  uint64_t byte = page / 8;
  uint64_t bit = page % 8;
  bitmap[byte] |= (1<<bit);
}
void EmeraldMem_PMM_pmm_set_page_free(uint64_t page) {
    uint64_t byte = page / 8;
    uint64_t bit = page % 8;
    bitmap[byte] &= ~(1<<bit);
}
uint8_t EmeraldMem_PMM_pmm_is_page_used(uint64_t page){
  uint64_t byte = page / 8;
  uint64_t bit = page % 8;
  return (bitmap[byte] & (1<<bit)) >> bit;
}
uint64_t EmeraldMem_PMM_pmm_find_free_pages(uint64_t size){
  uint64_t needed_pages = (size + 0x1000 -1) / 0x1000;
  uint64_t found_pages = 0;
  uint64_t current_page = 0;
  for (uint64_t i = 0; i < memory_pages; i++) {
    if (!EmeraldMem_PMM_pmm_is_page_used(i)) {
        if (found_pages == 0) {
            current_page = i;
        }
        found_pages++;
    } else {
        found_pages = 0;
    }

    if (found_pages >= needed_pages) {
        return current_page;
    }
  }
  kprint("[FAILED] Free memory not found!",0x7);
  while (1) {
    asm volatile("hlt");
}
}
void *EmeraldMem_PMM_pmm_alloc(uint64_t size){
  uint64_t needed_pages = (size + 0x1000 - 1) / 0x1000;
  uint64_t free_page = EmeraldMem_PMM_pmm_find_free_pages(size);
  for (uint64_t i = 0; i < needed_pages; i++){
    EmeraldMem_PMM_pmm_set_page_used(free_page + i);
  }
}

void EmeraldMem_PMM_pmm_unalloc(void *addr, uint64_t size){
  uint64_t page = (uint64_t) addr / 0x1000;
  uint64_t pages = (size + 0x1000 - 1) / 0x1000;
  for (uint64_t i = 0; i < pages; i++) {
    EmeraldMem_PMM_pmm_set_page_free(page + i);
}
}
