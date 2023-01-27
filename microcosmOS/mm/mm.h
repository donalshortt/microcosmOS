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
} heap_metadata_element;

struct heap_metadata {
	struct heap_metadata_element* first_element;
} heap_metadata;

void* kmalloc(int size);

#endif
