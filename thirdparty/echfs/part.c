#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "part.h"

struct mbr_entry {
	uint8_t status;
	uint8_t chs_first_sect[3];
	uint8_t type;
	uint8_t chs_last_sect[3];
	uint32_t first_sect;
	uint32_t sect_count;
} __attribute__((packed));

int mbr_get_part(struct part *ret, FILE *file, int partition) {
    fseek(file, 0x1BE, SEEK_SET);
    struct mbr_entry entries[4];
    fread(entries, sizeof(struct mbr_entry), 4, file);

    if (!entries[partition].type)
        return -1;

    ret->first_sect = entries[partition].first_sect;
    ret->sect_count = entries[partition].sect_count;

    return 0;
}

struct gpt_table_header {
    // the head
    char     signature[8];
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t _reserved0;

    // the partitioning info
    uint64_t my_lba;
    uint64_t alternate_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;

    // the guid
    struct {
        uint64_t low;
        uint64_t high;
    } disk_guid;

    // entries related
    uint64_t partition_entry_lba;
    uint32_t number_of_partition_entries;
    uint32_t size_of_partition_entry;
    uint32_t partition_entry_array_crc32;
} __attribute__((packed));

#define GPT_ATTR_IMPORTANT (1u << 0u)
#define GPT_ATTR_DONT_MOUNT (1u << 1u)
#define GPT_ATTR_FOR_LEGACY (1u << 2u)

struct gpt_entry {
    struct {
        uint64_t low;
        uint64_t high;
    } partition_type_guid;

    struct {
        uint64_t low;
        uint64_t high;
    } unique_partition_guid;

    uint64_t starting_lba;
    uint64_t ending_lba;

    uint64_t attributes;

    uint16_t partition_name[36];
} __attribute__((packed));


int gpt_get_part(struct part *ret, FILE *file, int partition) {
    struct gpt_table_header header = {0};

    // read header, located after the first block
    fseek(file, 512, SEEK_SET);
    fread(&header, 1, sizeof(header), file);

    // check the header
    // 'EFI PART'
    if (strncmp(header.signature, "EFI PART", 8)) return -1;
    if (header.revision != 0x00010000) return -1;

    // parse the entries if reached here
    if (partition >= header.number_of_partition_entries) return -1;

    struct gpt_entry entry = {0};
    fseek(file, (header.partition_entry_lba * 512) + (partition * sizeof(entry)), SEEK_SET);
    fread(&entry, 1, sizeof(entry), file);

    if (entry.unique_partition_guid.low  == 0 &&
        entry.unique_partition_guid.high == 0) return -1;

    ret->first_sect = entry.starting_lba;
    ret->sect_count = (entry.ending_lba - entry.starting_lba) + 1;

    return 0;
}
