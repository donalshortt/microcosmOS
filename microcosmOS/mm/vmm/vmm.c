#include "vmm.h"

inline uint64_t pe_set_flag(pe pe, uint64_t flag) { return pe |= flag; };
inline uint64_t pe_del_flag(pe pe, uint64_t flag) { return pe &= ~flag; };
inline uint64_t pe_set_addr(pe pe, uint64_t addr) { return pe |= addr << 12; };

//TODO: Standardise return codes!
/*int vmm_alloc_page(pte* pte)
{
    uintptr_t page_addr = pmm_alloc_block();

    if(!page_addr) {
        //TODO: Error
        return 1;
    }

    pe_set_flag(pte, PAGE_PRESENT);
    pe_set_flag(pte, PAGE_WRITEABLE);
    pe_set_addr(pte, page_addr);

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
}*/

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

void flush_tlb()
{
	__asm__ __volatile__("wbinvd" : : : "memory");
}

// Sets up an entry for a virtual address
// something is extremely fishy here
void vmm_map_page(uintptr_t phys, uintptr_t virt)
{   
    struct PML4* pml4 = (struct PML4*)get_current_pml4();

    uint64_t pml4_i  = PML4_GET_INDEX(virt);
    uint64_t pdpt_i  = PDPT_GET_INDEX(virt);
    uint64_t pd_i    = PD_GET_INDEX(virt);
    uint64_t pt_i    = PT_GET_INDEX(virt);

    struct PDPT* pdpt = 0;
    struct PD* pd = 0;
    struct PT* pt = 0;

    if ((pml4->entries[pml4_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pdpt = (struct PDPT*) pmm_alloc_block();
        //kmemset(pdpt, 0, PDPT_SIZE);
        
		pml4e pml4e = 0;

		pml4e = pe_set_addr(pml4e, (uint64_t)pdpt);
		pml4e = pe_set_flag(pml4e, PAGE_PRESENT);
		pml4e = pe_set_flag(pml4e, PAGE_WRITEABLE);

		pml4->entries[pml4_i] = pml4e;
    } else {
		pdpt = (struct PDPT*)pml4->entries[pml4_i];
    }

    if ((pdpt->entries[pdpt_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pd = (struct PD*) pmm_alloc_block();
        //kmemset(pd, 0, PD_SIZE);
        
		pdpte pdpte = 0;

		pdpte = pe_set_addr(pdpte, (uint64_t)pd);
		pdpte = pe_set_flag(pdpte, PAGE_PRESENT);
		pdpte = pe_set_flag(pdpte, PAGE_WRITEABLE);

		pdpt->entries[pdpt_i] = pdpte;
    } else {
		pd = (struct PD*)pdpt->entries[pdpt_i];
    }
    
    if ((pd->entries[pd_i] & PAGE_PRESENT) != PAGE_PRESENT) {
		pt = (struct PT*) pmm_alloc_block();
        //kmemset(pt, 0, PT_SIZE);
        
		pde pde = 0;

		pde = pe_set_addr(pde, (uint64_t)pt);
		pde = pe_set_flag(pde, PAGE_PRESENT);
		pde = pe_set_flag(pde, PAGE_WRITEABLE);

		pd->entries[pd_i] = pde;
    } else {
		pt = (struct PT*)pd->entries[pd_i];
    }

	pte pte = 0;

	pte = pe_set_addr(pte,(uint64_t)phys);
	pte = pe_set_flag(pte, PAGE_PRESENT);
	pte = pe_set_flag(pte, PAGE_WRITEABLE);

	pt->entries[pt_i] = pte;

	// TODO: don't flush the entire TLB
	flush_tlb();
}

