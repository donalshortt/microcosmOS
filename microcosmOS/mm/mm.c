#include "../includes/pmm.h"
#include "../includes/vmm.h"
#include "../includes/types.h"

//TODO: Remove this shit
//struct PMM* pmm_state;

/*void* kmalloc(int size)
{
    int no_blocks = 0;

    if (!(size % 4096)) {
        no_blocks = size / 4096;
    } else {
        no_blocks = (size / 4096) + 1;
    }

    // Check if there is enough memory available (first fit?)
    if (size == 1) {
        void* phys_alloced = pmm_alloc_block(pmm_state);
    } else {

    }

}

void kfree(void* ptr)
{

}*/
