#include "mm.h"
#include "pmm/pmm.h"
#include "vmm/vmm.h"

struct heap_block {
	int size;
	char is_free;
	struct heap_block* next;
	uintptr_t data;
};

struct heap_block* heap = {0};

struct heap_block* find_first_fit(int size)
{
	struct heap_block* current_block = heap;

	while (current_block->next != NULL) {
		if (current_block->size > size) {
			return current_block;
		}

		current_block = current_block->next;
	}

	return current_block;
}

void heap_init()
{
	if (heap->data == 0) {
		int wow = 42;
	} else {
		int sadwow = 24;
	}
}

void* kmalloc(int size)
{
	if (heap->data == 0) {
		uintptr_t frame_addr = pmm_alloc_block(heap);
		vmm_map_page(frame_addr, MAIN_MEMORY_START);

		struct heap_block* metadata = (struct heap_block*) MAIN_MEMORY_START;
		metadata->size = size;
		metadata->is_free = 0;
		metadata->next = NULL;
		metadata->data = MAIN_MEMORY_START + 4096;

		int no_blocks = size / 4096;
		if (size % 4096 != 0) 
			no_blocks++;
		
		for (int i = 0; i < no_blocks; i++) {
			frame_addr = pmm_alloc_block(heap);
			//vmm_map_page(frame_addr, (MAIN_MEMORY_START + i * 0x1000));
		}

		return (void*)metadata->data;
	}

	return 0;
}

void kfree(void* ptr)
{

}
