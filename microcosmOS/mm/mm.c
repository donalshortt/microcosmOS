#include "mm.h"

struct heap_metadata heap_metadata;

void heap_init() {

}

void* kmalloc(int size)
{
    int no_blocks = 0;

    if (size % 4096) {
        no_blocks = (size / 4096) + 1;
    } else {
        no_blocks = size / 4096;
    }

    // Check if there is enough memory available (first fit?)
    if (no_blocks == 1) {
        void* phys_alloced = pmm_alloc_block();
    } else {
    
    }

    return 0;
}

void kfree(void* ptr)
{

}
