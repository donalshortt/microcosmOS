#include "vmm.h"

inline void pe_set_flag(pe* pe, uint64_t flag) { *pe |= flag; };
inline void pe_del_flag(pe* pe, uint64_t flag) { *pe &= ~flag; };
inline void pe_set_addr(pe* pe, uint64_t addr) { 
    *pe |= PAGE_ADDR; 
    *pe &= addr; 
};

//TODO: Standardise return codes!
int vmm_alloc_page(pte* pte)
{
    void* page_addr = pmm_alloc_block();

    if(!page_addr) {
        //TODO: Error
        return 1;
    } 

    pe_set_flag(pte, PAGE_PRESENT);
    pe_set_flag(pte, PAGE_WRITEABLE);
    pe_set_addr(pte, (uint64_t)page_addr);

    return 0;
}

void vmm_dealloc_page(pe* pe)
{
    void* addr = (void*)(*pe & PAGE_ADDR);

    if (!addr) {
        //TODO: Error
        return;
    }

    pmm_dealloc_block(addr);
    pe_del_flag(pe, PAGE_PRESENT);
}

__attribute__((unused)) void vmm_set_CR3(__attribute__((unused)) uint64_t pml4)
{
    __asm__ ( "movq [pml4], %rax" );
    __asm__ ( "movq %rax, %cr3" );
}

//TODO: Remove this shit
struct PML4* CURRENT_PML4;

__attribute__((unused)) int vmm_switch_pml4(struct PML4* pml4)
{
    if(!pml4) {
        //TODO: Error
        return 1;
    }

    CURRENT_PML4 = pml4;
    vmm_set_CR3((uint64_t)pml4);

	return 0;
}


void* get_current_pml4()
{
    void* rv;
    __asm__ __volatile__(
        "mov %%cr3, %0"
        : "=r"(rv)
    );
    return rv;
}

// Sets up an entry for a virtual address
void vmm_map_page(uint64_t phys, uint64_t virt)
{   
    struct PML4* pml4 = (struct PML4*)get_current_pml4();

    uint64_t pml4_i  = PML4_GET_INDEX((uint64_t)virt);
    uint64_t pdpte_i = PDPT_GET_INDEX((uint64_t)virt);
    uint64_t pde_i   = PD_GET_INDEX((uint64_t)virt);
    uint64_t pte_i   = PT_GET_INDEX((uint64_t)virt);

    struct PDPT* pdpt = 0;
    struct PD* pd = 0;
    struct PT* pt = 0;

    if ((pml4->entries[pml4_i] & PAGE_PRESENT) != PAGE_PRESENT) {
        pdpt = (struct PDPT*) pmm_alloc_block();
        kmemset(pdpt, 0, PDPT_SIZE);
        
        pml4e* pml4e = &pml4->entries[pml4_i];

        pe_set_flag(pml4e, PAGE_PRESENT);
        pe_set_flag(pml4e, PAGE_WRITEABLE);
        pe_set_addr(pml4e, (uint64_t)pdpt);
    } else {
        pdpt = (struct PDPT*)&pml4->entries[pml4_i];
    }

    if ((pdpt->entries[pdpte_i] & PAGE_PRESENT) != PAGE_PRESENT) {
        pd = (struct PD*) pmm_alloc_block();
        kmemset(pd, 0, PD_SIZE);
        
        pdpte* pdpte = &pdpt->entries[pdpte_i];
        pe_set_flag(pdpte, PAGE_PRESENT);
        pe_set_flag(pdpte, PAGE_WRITEABLE);
        pe_set_addr(pdpte, (uint64_t)pd);
    } else {
        pd = (struct PD*)&pdpt->entries[pdpte_i];
    }
    
    if ((pdpt->entries[pde_i] & PAGE_PRESENT) != PAGE_PRESENT) {
        pt = (struct PT*) pmm_alloc_block();
        kmemset(pt, 0, PT_SIZE);
        
        pte* pte = &pt->entries[pde_i];
        pe_set_flag(pte, PAGE_PRESENT);
        pe_set_flag(pte, PAGE_WRITEABLE);
        pe_set_addr(pte, (uint64_t)pt);
    } else {
        pt = (struct PT*)&pd->entries[pde_i];
    }

    pte* pte = &pt->entries[pte_i];

    pe_set_flag(pte, PAGE_PRESENT);
    pe_set_flag(pte, PAGE_WRITEABLE);
    pe_set_addr(pte,(uint64_t)phys);
}

