#ifndef __MM_H__
#define __MM_H__

#include "pmm/pmm.h"
#include "vmm/vmm.h"
#include "../util/types.h"

#define MAIN_MEMORY_START  0x200000 // 2mb

struct heap_metadata_element {
	char free;
	int size; // in blocks
	struct heap_chunk* next;
	void* data;
};

struct heap_metadata {
	struct heap_metadata_element* first_element;
};

void* kmalloc(int size);

void heap_init();

#endif
