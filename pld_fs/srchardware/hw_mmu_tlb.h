/* ------------------------------
   $Id: hw_disk.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_mmu_tlb.h

   Interface du module de gestion de la TLB (partie de la MMU)

*/

#ifndef _MMU_HANDLER_H_
#define _MMU_HANDLER_H_

/* Registers */
extern int TLB_ADD_ENTRY;	/* Command register for adding a line in the TLB */
extern int TLB_DEL_ENTRY;	/* Command register for removing a line in the TLB */
extern int TLB_ENTRIES;		/* All the entries in the TLB */
extern int TLB_ENTRIES_MAX;	/* End of the TLB entries */

/* Consts */
extern int TLB_SIZE;		/* Number of entries in the TLB */

/* Registered functions */
extern void out_TLB_ADD_ENTRY(int port, int value);
extern void out_TLB_DEL_ENTRY(int port, int value);
extern int in_TLB_ENTRIES(int port);
extern void out_TLB_ENTRIES(int port, int value);

/* Shared functions */
extern void tlb_init();
extern void tlb_reset();
extern void tlb_close();

/* manage cpu mode changes */
extern void mmuSwitchToUserMode();
extern void mmuSwitchToMasterMode();

#endif
