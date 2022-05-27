#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// BOOT STACK

#define BOOT_STACK_SIZE 0x4000
#define BOOT_STACK_ALIGN 0x1000

// CONTROL REGISTERS

#define CR0_PROTECTED_MODE (1 << 0) // Enable protected mode
#define CR0_EXT_TYPE (1 << 4)       // Specifies which math coprocessor is used, at least on the i386 -> these days it's called the FPU (floating point unit)
                                    // TODO: Double check whether this is necessary
#define CR0_PAGING_ENABLE (1 << 31) // Enable paging
#define CR0_FINAL           \
    (                       \
    CR0_PROTECTED_MODE |    \
    CR0_EXT_TYPE       |    \
    CR0_PAGING_ENABLE       \
    )

#define CR4_PAE (1 << 5)            // Enable PAE
                                    // "If paging is enabled then PAE must also be enabled before entering long mode. Attempting to enter long mode with CR0.PG set and CR4.PAE cleared will trigger a general protection fault."
                                    // TODO: Double check whether this is necessary
#define CR4_FINAL           \
    (                       \
    CR4_PAE                 \
    )

#define MSR_EFER_LME (1 << 8)
#define MSR_EFER_FINAL      \
    (                       \
    MSR_EFER_LME            \
    )

#define MSR_EFER_ADDR 0xC0000080

// KERNEL MEM ADDRESSES

#define OS_PHYS_START 0x0000000000400000
#define OS_VIRT_START 0xFFFFFFFF80400000

// PHYS MEM MANAGEMENT

#define BLOCK_SIZE 4096
#define BLOCKS_PER_QWORD 64
#define B_TO_BLOCKS(x) (x / BLOCK_SIZE)


//#define 

#endif


