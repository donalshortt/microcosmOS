#ifndef __PMM_H__
#define __PMM_H__

struct PMM
{
    long mem_size;
    int used_blocks;
    int max_blocks;
    unsigned long* pmmap;
    int pmmap_size;
};

typedef struct PMM PMM;

unsigned long get_available_mem(unsigned long boot_info_addr);
void init_pmm(struct PMM* pmm, unsigned long pmmap_addr, unsigned long boot_info_addr);
void test(struct PMM* pmm);

#endif
