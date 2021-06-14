#ifndef __PART_H__
#define __PART_H__

#include <stdint.h>
#include <stdio.h>

struct part {
	uint64_t first_sect;
	uint64_t sect_count;
};

int mbr_get_part(struct part *ret, FILE *file, int partition);
int gpt_get_part(struct part *ret, FILE *file, int partition);

#endif
