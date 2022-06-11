#include "../includes/vmm.h"
#include "../includes/pmm.h"

inline void pe_set_flag(pte* pte, uint64_t flag) { *pte |= flag; };
inline void pe_del_flag(pte* pte, uint64_t flag) { *pte &= ~flag; };
inline void pe_set_addr(pte* pte, uint64_t addr) { *pte = (*pte & PAGE_ADDR) | addr; };

//TODO: Standardise return codes!

//TODO: Remove this shit
struct PMM* pmm_state;

int vmm_alloc_page(pte* pte)
{
    if(!pmm_state) {
        //TODO: Error
        return 1;
    }

    void* page_addr = pmm_alloc_block(pmm_state);

    if(!page_addr) {
        //TODO: Error
        return 1;
    } 

    pe_set_flag(pte, PAGE_PRESENT);
    pe_set_addr(pte, (uint64_t)page_addr);

    return 0;
}

void vmm_dealloc_page(pte* pte)
{
    void* addr = (void*)(*pte & PAGE_ADDR);

    if (!addr) {
        //TODO: Error
        return;
    }

    pmm_dealloc_block(addr, pmm_state);
    pe_del_flag(pte, PAGE_PRESENT);
}

inline pte* vmm_lookup_pte(struct PT* pt, vaddr vaddr)
{
    if(!pt) {
        //TODO: Error
        return 0;
    }

    return &pt->entries[ PT_GET_INDEX(vaddr) ];
}

inline pde* vmm_lookup_pde(struct PD* pd, vaddr vaddr)
{
    if(!pd) {
        //TODO: Error
        return 0;
    }
    
    return &pd->entries[ PD_GET_INDEX(vaddr) ];
}

__attribute__((unused)) void vmm_set_CR3(uint64_t pml4)
{
    __asm__ ( "movq [pml4], %rax" );
    __asm__ ( "movq %rax, %cr3" );
}

//TODO: Remove this shit
uint64_t CURRENT_PML4;

__attribute__((unused)) int vmm_switch_pml4(struct PML4* pml4)
{
    if(!pml4) {
        //TODO: Error
        return 0;
    }

    CURRENT_PML4 = pml4;
    vmm_set_CR3((uint64_t)pml4);
}

// Sets up a pte for a virtual address

void vmm_map_page(void* virt, void* phys)
{
    struct PML4* pml4 = CURRENT_PML4;
    pml4e* pml4e = &pml4->entries[ PML4_GET_INDEX( (uint64_t) virt) ];
    
    struct PDPT* pdpt = (struct PDPT* )pml4e;
    pdpte* pdpte = &pdpt->entries[ PDPT_GET_INDEX( (uint64_t) virt) ];
                                
    struct PD* pd = (struct PD* )pdpte;
    pde* pde = &pd->entries[ PD_GET_INDEX( (uint64_t) virt ) ];

        

}

