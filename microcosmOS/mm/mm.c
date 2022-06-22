#include "../includes/pmm.h"
#include "../includes/vmm.h"
#include "../includes/types.h"

void* kmalloc(int size)
{
    int no_blocks = 0;

    if (!(size % 4096)) {
        no_blocks = size / 4096;
    } else {
        no_blocks = (size / 4096) + 1;
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
