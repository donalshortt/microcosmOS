#ifndef __PMM_H__
#define __PMM_H__

#include "../../util/multiboot2.h"
#include "../../util/constants.h"
#include "../../util/types.h"

struct PMM
{
    unsigned long mem_size;
    int used_blocks;
    int max_blocks;
    unsigned long* pmmap;
    int pmmap_size;
};

extern struct PMM* pmm_state;

void* kmemset(void* addr, int set, int len);
void setup_pmm(unsigned long boot_info_addr);

uintptr_t pmm_alloc_block();
void pmm_dealloc_block(uintptr_t ptr);

long get_first_free_block();

#endif
