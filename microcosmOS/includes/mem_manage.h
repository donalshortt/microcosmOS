#ifndef __MEM_MANAGE_H__
#define __MEM_MANAGE_H__

struct Block
{
    Block* next;
    Block* prev;
    bool allocated;
    int size;
};

void init_heap(unsigned long boot_info_addr);
void* kmalloc(int size);
void kfree(void* ptr);

#endif
