#ifndef __MM_H__
#define __MM_H__

#include "pmm/pmm.h"
#include "vmm/vmm.h"
#include "../util/types.h"

#define FOUR_KiB 0x1000
#define ONE_MiB 0x100000
#define ONE_GiB 0x40000000

#define MAIN_MEMORY_START 0xFFFFFFFF81000000 // OS_VIRT_START + 0x600000 
#define PAGING_ZONE_PHYS_START 0x600000
#define PAGING_ZONE_PHYS_END   0x40600000

enum mem_zone{page, heap};

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
