#ifndef __PMM_H__
#define __PMM_H__

struct Block
{
    Block* next;
    Block* prev;
    bool allocated;
    int size;
};

struct PMM
{
    long long mem_size;
    int used_blocks;
    int max_blocks;
    unsigned int* pmmap;
    int pmmap_size;
};

void init_pmm(unsigned long boot_info_addr);

#endif
