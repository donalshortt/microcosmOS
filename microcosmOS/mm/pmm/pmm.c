#include "pmm.h"

struct PMM* pmm_state;

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

// WARNING: This might be a bit too close to the implementation on the multiboot speciication examples
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



void pmm_init(unsigned long pmmap_addr, unsigned long boot_info_addr)
{
    unsigned long total_mem = get_available_mem(boot_info_addr);

    total_mem = total_mem + 0;

    pmm_state->mem_size = get_available_mem(boot_info_addr);
    pmm_state->max_blocks = B_TO_BLOCKS(pmm_state->mem_size);
    pmm_state->used_blocks = pmm_state->max_blocks;
    pmm_state->pmmap = (unsigned long*) pmmap_addr;
    pmm_state->pmmap_size = pmm_state->max_blocks / BLOCKS_PER_QWORD; // number of bits in a ulong is 64, so how many of these there are
    
    // If the no. of blocks is not divisible by 64, we will have some at end end so these must be accounted for
    if ((pmm_state->max_blocks % BLOCKS_PER_QWORD) != 0) {
        pmm_state->pmmap_size++;
    }
    
    kmemset(pmm_state->pmmap, 0, pmm_state->pmmap_size);
}

// Sets the bit -> The memory at the index is in use

static inline void bit_set(unsigned long* bitmap, int bit)
{
    bitmap[bit / 64] |= (1 << (bit % 64));
}

// Unsets the bit -> The memory at the index in not in use
// TODO: Test if I can make the bitset 0 and remove the complement

static inline void bit_unset(unsigned long* bitmap, int bit)
{
    bitmap[bit / 64] &= ~(1 << (bit % 64));
}

static inline char bit_test(unsigned long* bitmap, int bit)
{
    return bitmap[bit / 64] & (1 << (bit % 64));
}

//  Init: makes address space ready for allocation and vice versa 

void pmm_init_deinit_space(unsigned long base_addr, unsigned long mem_size, char i_or_d)
{                                                                                                                           
    unsigned int no_blocks = mem_size / BLOCK_SIZE;                                                                         
    unsigned int alignment = base_addr / BLOCK_SIZE;                                                                        

    if (i_or_d == SPACE_INIT) {
        for (unsigned int i = 0; i < no_blocks; i++) {
            bit_unset(pmm_state->pmmap, alignment++);
            pmm_state->used_blocks--;
        }
    } else {
        for (unsigned int i = 0; i < no_blocks; i++) {
            bit_set(pmm_state->pmmap, alignment++);
            pmm_state->used_blocks++;
        }
    }

    if (i_or_d == SPACE_INIT)
        bit_set(pmm_state->pmmap, 0);
}

// TODO: This is copied from get_total_mem -> try to reduce code duplication

void pmm_init_available_mem(unsigned long boot_info_addr)
{
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
                if (((struct multiboot_mmap_entry *) mmap)->type == MULTIBOOT_MEMORY_AVAILABLE) {
                    pmm_init_deinit_space(mmap->addr, mmap->len, SPACE_INIT);                    
                }
            }
        }
    }
}

// Deinits the space associated with the kernel boot, kernel proper and pmmap

void pmm_deinit_used_spaces()
{
    extern char* _kernel_physical_start;
    extern char* _kernel_size;
    extern char* _pmm_start;

    pmm_init_deinit_space((unsigned long) &_kernel_physical_start, (unsigned long) &_kernel_size, SPACE_DEINIT);
    pmm_init_deinit_space((unsigned long) &_pmm_start, pmm_state->pmmap_size, SPACE_DEINIT);
}

long get_first_free_block()
{
    int extra_bits = pmm_state->pmmap_size % 64;

    for (long i = 0; i < (pmm_state->pmmap_size / 64); i++) {
        for (long j = 0; j < 64; j++) {
                if (!(pmm_state->pmmap[i] & (1 << j)))
                    return (i * 64) + j;
        }
    }

    if (extra_bits) {
        for (int i = 0; i < extra_bits; i++) {
            if (!(pmm_state->pmmap[pmm_state->pmmap_size / 64] & (1 << i)))
                return ((pmm_state->pmmap_size / 64) * 64) + i;
        }
    }
        
    return -1;
}

void* pmm_alloc_block()
{
    if (pmm_state->max_blocks - pmm_state->used_blocks <= 0) {
        /* TODO: ERROR no mem */
        return NULL;
    }

    long index = get_first_free_block();    

    bit_set(pmm_state->pmmap, index);

    pmm_state->used_blocks++;

    return (void*) (index * BLOCK_SIZE);
}

void pmm_dealloc_block(void* ptr)
{
    if (ptr == NULL) {
        // TODO: ERROR null ptr
        return;
    }

    long index = (unsigned long) ptr / BLOCK_SIZE;
    bit_unset(pmm_state->pmmap, index);

    pmm_state->used_blocks++;
}

void setup_pmm(unsigned long boot_info_addr)
{
	pmm_state = 0;

    extern char* _pmm_start;
    pmm_init((unsigned long) &_pmm_start, boot_info_addr);
    
    pmm_init_available_mem(boot_info_addr);
    pmm_deinit_used_spaces();
}
