#include "types.h"

#ifndef __VMM_H__
#define __VMM_H__

#define ENTRIES_PER_PT   512
#define ENTRIES_PER_PD   512
#define ENTRIES_PER_PDPT 512
#define ENTRIES_PER_PML4 512

#define PAGE_PRESENT    0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000001
#define PAGE_WRITEABLE  0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000010
#define PAGE_MODE       0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000100
#define PAGE_ADDR       0b/**/00000000/**/11111111/**/11111111/**/11111111/**/11111111/**/11111111/**/11110000/**/00000000

#define PT_GET_INDEX(x)   //TODO
#define PD_GET_INDEX(x)   //TODO
#define PDPT_GET_INDEX(x) //TODO
#define PML4_GET_INDEX(x) //TODO

struct PT 
{
    pte entries[ENTRIES_PER_PT];
};

struct PD
{
    pte entries[ENTRIES_PER_PD];
};

struct PDPT
{
    pte entries[ENTRIES_PER_PDPT];
};

struct PML4
{
    pte entries[ENTRIES_PER_PML4];
};

void pte_set_flag(pte* pte, uint64_t mask);
void pte_del_flag(pte* pte, uint64_t mask);

int vmm_alloc_page(pte* pte);
void vmm_dealloc_page(pte* pte);

pte* vmm_lookup_pte(PT* pt, vaddr vaddr);
pte* vmm_lookup_pde(PD* pd, vaddr vaddr);
pte* vmm_lookup_pdpte(PDPT* pdpt, vaddr vaddr);
pte* vmm_lookup_pml4e(PML4* pml4, vaddr vaddr);

int vmm_switch_pml4(PML4* pml4);

void vmm_switch_tlbe(vaddr vaddr);

void vmm_map_page(void* virt, void* phys);

void vmm_init();

#endif
