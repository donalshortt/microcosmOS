#include "includes/pmm.h"
#include "includes/multiboot2.h"
#include "includes/constants.h"

void* kmemset(void* addr, int set, int len)
{
    if (addr == 0)
        return 0;

    char* temp = (char*) addr;

    while(len > 0 && *temp) {
        *temp = set;
        temp++;
        len--;
    }

    return addr;
}

//TODO: This might be a bit too close to the implementation on the multiboot speciication examples
unsigned long get_available_mem(unsigned long boot_info_addr)
{
    unsigned long long total_mem = 0;
    struct multiboot_tag* mb_tag;
    
    for (mb_tag = (struct multiboot_tag *) (boot_info_addr + 8);
            mb_tag->type != MULTIBOOT_TAG_TYPE_END;
            mb_tag = (struct multiboot_tag *) ((multiboot_uint8_t *) mb_tag 
                + ((mb_tag->size + 7) & ~7)))
    {
        if (mb_tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            multiboot_memory_map_t *mmap;

            for (mmap = ((struct multiboot_tag_mmap *) mb_tag)->entries;
                    (multiboot_uint8_t *) mmap 
                        < (multiboot_uint8_t *) mb_tag + mb_tag->size;
                    mmap = (multiboot_memory_map_t *) ((unsigned long) mmap 
                        + ((struct multiboot_tag_mmap *) mb_tag)->entry_size))
            {
                if (((struct multiboot_mmap_entry *) mmap)->type == MULTIBOOT_MEMORY_AVAILABLE)
                    total_mem += ((struct multiboot_mmap_entry *) mmap)->len;
            }
        }
    }

    return total_mem;
}



void init_pmm(struct PMM* pmm, unsigned long pmmap_addr, unsigned long boot_info_addr)
{
    unsigned long total_mem = get_available_mem(boot_info_addr);

    total_mem = total_mem + 0;

    pmm->mem_size = get_available_mem(boot_info_addr);
    pmm->max_blocks = B_TO_BLOCKS(pmm->mem_size);
    pmm->used_blocks = pmm->max_blocks;
    pmm->pmmap = (unsigned long*) pmmap_addr;
    pmm->pmmap_size = pmm->max_blocks / BLOCKS_PER_QWORD; // number of bits in a ulong is 64, so how many of these there are
    
    // if there the no. of blocks is not divisible by 64, we will have some at end end so these must be accounted for
    if ((pmm->max_blocks % BLOCKS_PER_QWORD) != 0) {
        pmm->pmmap_size++;
    }
    
    kmemset(pmm->pmmap, 0x2A, pmm->pmmap_size);
}

void bit_set(unsigned long *bitmap, int bit) {
    bitmap[bit / 64] |= (1 << (bit % 64));
}


void bit_unset(unsigned long *bitmap, int bit) {
    bitmap[bit / 64] &= ~(1 << (bit % 64));
}

void pmm_init_space(struct PMM* pmm, unsigned long base_addr, unsigned int mem_size)                                        
{                                                                                                                           
    unsigned int no_blocks = mem_size / BLOCK_SIZE;                                                                         
    unsigned int alignment = base_addr / BLOCK_SIZE;                                                                        

    for (unsigned int i = 0; i < no_blocks; i++) {
        bit_unset(pmm->pmmap, alignment++);
        no_blocks--;
    }

    bit_set(pmm->pmmap, 0);
}

void test(struct PMM* pmm)
{
    bit_set(pmm->pmmap, 1);
    bit_unset(pmm->pmmap, 2);
    bit_set(pmm->pmmap,3);
}
