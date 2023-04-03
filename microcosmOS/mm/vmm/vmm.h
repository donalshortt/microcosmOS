#ifndef __VMM_H__
#define __VMM_H__

#include "../../util/types.h"
#include "../pmm/pmm.h"

#define ENTRIES_PER_PT   512
#define ENTRIES_PER_PD   512
#define ENTRIES_PER_PDPT 512
#define ENTRIES_PER_PML4 512

#define PML4_SIZE 4096
#define PDPT_SIZE 4096
#define PD_SIZE   4096
#define PT_SIZE   4096

#define PAGE_PRESENT   0x1
#define PAGE_WRITEABLE 0x2
#define PAGE_MODE      0x4
#define PAGE_2MB 	   (1 << 7)
#define PAGE_ADDR      0xFFFFFFFFFFF000

#define PML4_GET_INDEX(x) (((x) >> 39) & 0x1ff)
#define PDPT_GET_INDEX(x) (((x) >> 30) & 0x1ff)
#define PD_GET_INDEX(x)   (((x) >> 21) & 0x1ff)
#define PT_GET_INDEX(x)   (((x) >> 12) & 0x1ff)

struct PML4
{
    pml4e entries[ENTRIES_PER_PML4];
};

struct PDPT
{
    pdpte entries[ENTRIES_PER_PDPT];
};

struct PD
{
    pde entries[ENTRIES_PER_PD];
};

struct PT 
{
    pte entries[ENTRIES_PER_PT];
};

//struct PML4* CURRENT_PML4 = 0;

uint64_t pe_set_flag(pe pe, uint64_t mask);
uint64_t pe_del_flag(pe pe, uint64_t mask);
uint64_t pe_set_addr(pe pe, uint64_t addr);

int vmm_alloc_page(pte* pte);
void vmm_dealloc_page(pte* pte);

pte* vmm_lookup_pte(struct PT* pt, vaddr vaddr);
pte* vmm_lookup_pde(struct PD* pd, vaddr vaddr);
pte* vmm_lookup_pdpte(struct PDPT* pdpt, vaddr vaddr);
pte* vmm_lookup_pml4e(struct PML4* pml4, vaddr vaddr);

int vmm_switch_pml4(struct PML4* pml4);

void vmm_remove_tlbe(vaddr vaddr);

void vmm_map_page(uint64_t virt);

void vmm_init();

#endif
