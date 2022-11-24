#ifndef __PMM_H__
#define __PMM_H__

typedef struct PMM
{
    unsigned long mem_size;
    int used_blocks;
    int max_blocks;
    unsigned long* pmmap;
    int pmmap_size;
} pmm_state;

// struct PMM* pmm_state;

void* kmemset(void* addr, int set, int len);
void setup_pmm(unsigned long boot_info_addr);

void* pmm_alloc_block();
void pmm_dealloc_block(void* ptr);

long get_first_free_block();

#endif
