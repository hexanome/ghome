/* ------------------------------
   $Id: hw_disk.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_mmu.h

   Interface du module de gestion de la MMU

*/

#ifndef _MMU_H_
#define _MMU_H_

#include <signal.h>
#include "hw_mmu_tlb.h"

/* number of bits used for the address of physical memory pages */
#define PM_NBITS 8
/* number of bits used for the address of virtual memory pages */
#define VM_NBITS 12
/* number of bits used for addresses inside a page */
#define ADDR_NBITS 12

/* page size: 4 KB */
#define PAGE_SIZE (1 << ADDR_NBITS)
/* size of the physical memory: 16 MB */
#define PM_SIZE (PAGE_SIZE * (1 << PM_NBITS))
/* size of the virtual memory: 256 MB */
#define VM_SIZE (PAGE_SIZE * (1 << VM_NBITS))

/* Registers */
extern int MMU_CMD;		/* Command register */
extern int MMU_FAULT_ADDR;  /* Last Faulting address caught */

/* Consts */
extern int MMU_ENABLE;		/* Is MMU simulated? */
extern int MMU_IRQ; /* ID of the MMU IRQ */

/* Physical and virtual memory */
extern void *physical_memory;
extern void *virtual_memory;
/* Last faulting address */
extern void *mmu_faulting_addr;
/* Is a given pm page dirty? */
extern char dirty_mp_pages[1 << PM_NBITS];

/* Registered functions */
extern void out_MMU_CMD(int port, int value);
extern int in_MMU_FAULT_ADDR(int port);

/* MMU initialization */
extern int mmu_init();
/* close MMU */
extern int mmu_close();
/* tests if the address is in the virtual memory address space */
extern char vm_contains(void *addr);

#endif
