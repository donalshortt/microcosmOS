#include "types.h"

#ifndef __VMM_H__
#define __VMM_H__

#define ENTRIES_PER_PT   512
#define ENTRIES_PER_PD   512
#define ENTRIES_PER_PDPT 512
#define ENTRIES_PER_PML4 512

//#define PAGE_PRESENT    0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000001
//#define PAGE_WRITEABLE  0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000010
//#define PAGE_MODE       0b/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000000/**/00000100
//#define PAGE_ADDR       0b/**/00000000/**/11111111/**/11111111/**/11111111/**/11111111/**/11111111/**/11110000/**/00000000

#define PAGE_PRESENT   0x1
#define PAGE_WRITEABLE 0x2
#define PAGE_MODE      0x4
#define PAGE_ADDR      0xFFFFFFFFFFF000

//TODO: Change to binary?
#define PT_GET_INDEX(x)   (((x) >> 12) & 0x3ff)
#define PD_GET_INDEX(x)   (((x) >> 21) & 0x3ff)
#define PDPT_GET_INDEX(x) (((x) >> 30) & 0x3ff)
#define PML4_GET_INDEX(x) (((x) >> 39) & 0x3ff)

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

struct PML4* CURRENT_PML4 = 0;

void pe_set_flag(pte* pte, uint64_t mask);
void pe_del_flag(pte* pte, uint64_t mask);
void pe_set_addr(pte* pte, uint64_t addr);

int vmm_alloc_page(pte* pte);
void vmm_dealloc_page(pte* pte);

pte* vmm_lookup_pte(struct PT* pt, vaddr vaddr);
pte* vmm_lookup_pde(struct PD* pd, vaddr vaddr);
pte* vmm_lookup_pdpte(struct PDPT* pdpt, vaddr vaddr);
pte* vmm_lookup_pml4e(struct PML4* pml4, vaddr vaddr);

int vmm_switch_pml4(PML4* pml4);

void vmm_remove_tlbe(vaddr vaddr);

void vmm_map_page(void* virt, void* phys);

void vmm_init();

#endif
