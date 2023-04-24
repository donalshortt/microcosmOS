#include "pmm/pmm.h"
#include "vmm/vmm.h"
#include "../util/types.h"

struct heap_block {
	int size;
	char is_free;
	struct heap_block* next;
	uintptr_t data;
};

struct heap_block* k_heap = NULL;

struct heap_block* find_first_fit(int size)
{
	struct heap_block* current_block = k_heap;

	while (current_block != 0) {
		if (current_block->size > size && current_block->is_free == TRUE) {
			return current_block;
		}

		current_block = current_block->next;
	}

	return NULL;
}

void add_heap_block(struct heap_block* heap_block)
{
	struct heap_block* current_block = k_heap;

	while (current_block != NULL) {
		if (current_block->next == NULL) {
			current_block->next = heap_block;
			return;
		}

		current_block = current_block->next;
	}
}

uintptr_t get_heap_end()
{
	struct heap_block* current_block = k_heap;

	while (current_block->next != NULL) {
		current_block = current_block->next;
	}

	return current_block->data + current_block->size;
}

void heap_init()
{
	if (k_heap->data == 0) {
		int wow = 42;
	} else {
		int sadwow = 24;
	}
}

//TODO: clean up this function jesuuuuuuuus
void* kmalloc(int size)
{
	int no_blocks = size / BLOCK_SIZE;
	if (size % 4096 != 0) 
		no_blocks++;

	if (k_heap == NULL) {
		uintptr_t frame_addr = pmm_alloc_block(heap);
		vmm_map_page(frame_addr, MAIN_MEMORY_START);

		struct heap_block* metadata = (struct heap_block*) MAIN_MEMORY_START;
		metadata->size = no_blocks * BLOCK_SIZE;
		metadata->is_free = FALSE;
		metadata->next = NULL;
		metadata->data = MAIN_MEMORY_START + BLOCK_SIZE;
		
		for (int i = 0; i < no_blocks; i++) {
			frame_addr = pmm_alloc_block(heap);
			vmm_map_page(frame_addr, metadata->data + (BLOCK_SIZE * i));
		}

		k_heap = metadata;

		return (void*)metadata->data;
	} else {
		struct heap_block* metadata = find_first_fit(size);

		if (metadata != NULL) {
			metadata->is_free = FALSE;
			return (void*)metadata->data;
		}

		uint64_t heap_end = get_heap_end();

		uintptr_t frame_addr = pmm_alloc_block(heap);
		vmm_map_page(frame_addr, heap_end);

		metadata = (struct heap_block*) heap_end;
		metadata->size = no_blocks * BLOCK_SIZE;
		metadata->is_free = FALSE;
		metadata->next = NULL;
		metadata->data = heap_end + BLOCK_SIZE;

		for (int i = 0; i < no_blocks; i++) {
			frame_addr = pmm_alloc_block(heap);
			vmm_map_page(frame_addr, metadata->data + (BLOCK_SIZE * i));
		}

		add_heap_block(metadata);

		return (void*)metadata->data;
	}

	return 0;
}

void kfree(void* ptr)
{

}
