#ifndef __PMM_H__
#define __PMM_H__

#include "../../util/multiboot2.h"
#include "../../util/constants.h"
#include "../../util/types.h"
#include "../mm.h"

#define FOUR_KiB 0x1000
#define ONE_MiB 0x100000
#define ONE_GiB 0x40000000

struct PMM
{
    unsigned long mem_size;
    int used_blocks;
    int max_blocks;
    unsigned long* pmmap;
    int pmmap_size;
};

extern struct PMM* pmm_state;

int kmemset(void* addr, int set, int len);
void setup_pmm(unsigned long boot_info_addr);

uintptr_t pmm_alloc_block(enum mem_zone);
void pmm_dealloc_block(uintptr_t ptr);

//TODO: change long to uint64_t?
long get_first_free_block(enum mem_zone);

#endif
