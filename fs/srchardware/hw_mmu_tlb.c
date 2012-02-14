/* ------------------------------
   $Id: hw_disk.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_mmu_tlb.c

   Module de gestion de la TLB (partie de la MMU)

*/

#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include "hw_tools.h"
#include "hw_mmu.h"

/* Registers */
int TLB_ADD_ENTRY;	/* Command register for adding a line in the TLB */
int TLB_DEL_ENTRY;	/* Command register for adding a line in the TLB */
int TLB_ENTRIES;		/* All the entries in the TLB */
int TLB_ENTRIES_MAX;	/* End of the TLB entries */

/* Consts */
int TLB_SIZE;		/* Number of entries in the TLB */

/* TLB entry */
struct tlb_entry_s {
	unsigned unused: 8;
	unsigned virt_page: VM_NBITS;
	unsigned phys_page: PM_NBITS;
	unsigned access_type: 3;
	unsigned is_active: 1;
};

/* globals */
static struct tlb_entry_s *tlb_entries = NULL;

/* erase all entries from TLB  */
void tlb_reset() {
	int i;
	if(tlb_entries) {
		for(i=0; i<TLB_SIZE ; i++) {
			tlb_entries[i].is_active = 0;
		}
	}
}

/* close TLB  */
void tlb_close() {
	if(tlb_entries) {
		tlb_reset();
		free(tlb_entries);
	}
}

/* TLB initialization  */
void tlb_init() {
	int i;
	tlb_close();
	tlb_entries = malloc(TLB_SIZE * sizeof(struct tlb_entry_s));
	for(i=0; i<TLB_SIZE ; i++) {
		tlb_entries[i].is_active = 0;
	}
}

/* calculate the real address of a pm page */
static void *ppage_to_addr(unsigned p_page) {
	return (void*)((p_page << 12) | (uintptr_t)physical_memory);
}

/* calculate the real address of a vm page */
static void *vpage_to_addr(unsigned v_page) {
	return (void*)((v_page << 12) | (uintptr_t)virtual_memory);
}

/* Add a vm->pm link in tlb */
static int tlb_link_vm(unsigned v_page, unsigned p_page, unsigned access_type){
	int i;
	struct tlb_entry_s *selected_entry = NULL;

	/* look for an entry */
	for(i=0; i<TLB_SIZE ; i++) {
		if(tlb_entries[i].is_active == 1 && tlb_entries[i].virt_page == v_page) {
			selected_entry = &tlb_entries[i];
			break;
		} else if(!selected_entry && tlb_entries[i].is_active == 0) {
			selected_entry = &tlb_entries[i];
		}
	}

	/* tlb is full, an entry has to be removed */
	if(selected_entry == NULL) {
		static unsigned rr_entry = 0;
		selected_entry = tlb_entries + (rr_entry++ % TLB_SIZE);
		selected_entry->is_active = 0;
	}
	
	/* set the new mapping and enable the entry */
	selected_entry->virt_page = v_page;
	selected_entry->phys_page = p_page;
	selected_entry->access_type = access_type;
	selected_entry->is_active = 1;
	
	return 1;
}

/* Remove all vm pages targeting a given pm page */
static int tlb_unlink_pm(unsigned p_page) {
	int i;
	/* for each entry targeting pm */
	for(i=0; i<TLB_SIZE ; i++) {
		if(tlb_entries[i].is_active == 1 && tlb_entries[i].phys_page == p_page) {
			tlb_entries[i].is_active = 0;
		}
	}
	
	return 1;
}

/* add a tlb entry */
void out_TLB_ADD_ENTRY(int port, int value) {
	struct tlb_entry_s entry = *((struct tlb_entry_s*)&value);
	tlb_link_vm(entry.virt_page, entry.phys_page, entry.access_type);
}

/* remove a tlb entry */
void out_TLB_DEL_ENTRY(int port, int value) {
	struct tlb_entry_s entry = *((struct tlb_entry_s*)&value);
	tlb_unlink_pm(entry.phys_page);
}

/* read a tlb entry */
int in_TLB_ENTRIES(int port) {
	return *((int*)tlb_entries + (port - TLB_ENTRIES));
}

/* write a tlb entry */
void out_TLB_ENTRIES(int port, int value) {
	tlb_unlink_pm(tlb_entries[port - TLB_ENTRIES].phys_page);
	*((int*)tlb_entries + (port - TLB_ENTRIES)) = value;
	tlb_link_vm(tlb_entries[port - TLB_ENTRIES].virt_page, tlb_entries[port - TLB_ENTRIES].phys_page, tlb_entries[port - TLB_ENTRIES].access_type);
}

/* manage cpu modes changes*/
void mmuSwitchToUserMode() {
	int i;
	for(i=0; i<TLB_SIZE ; i++) {
		if(tlb_entries[i].is_active) {
			/* unprotect this vm page for writting */
			if(mprotect(vpage_to_addr(tlb_entries[i].virt_page),PAGE_SIZE,PROT_WRITE)==-1) {
				trace(DEBUG_WARNING,"failed to protect memory at %p\n",vpage_to_addr(tlb_entries[i].virt_page));
			}
			/* copy data from physical page to the virtual page */
			memcpy(vpage_to_addr(tlb_entries[i].virt_page), ppage_to_addr(tlb_entries[i].phys_page), PAGE_SIZE);	
			/* set access rights to this vm page */
			if(mprotect(vpage_to_addr(tlb_entries[i].virt_page),PAGE_SIZE,tlb_entries[i].access_type)==-1) {
				trace(DEBUG_WARNING,"failed to protect memory at %p\n",vpage_to_addr(tlb_entries[i].virt_page));
			}
		}
	}
	if(mprotect(physical_memory,PM_SIZE,PROT_NONE)==-1) {
		trace(DEBUG_WARNING,"failed to protect memory at %p\n",physical_memory);
	}
}

void mmuSwitchToMasterMode() {
	int i;
	if(mprotect(physical_memory,PM_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC)==-1) {
		trace(DEBUG_WARNING,"failed to protect memory at %p\n",physical_memory);
	}
	for(i=0; i<TLB_SIZE ; i++) {
		if(tlb_entries[i].is_active) {
			/* unprotect this vm page for reading */
			if(mprotect(vpage_to_addr(tlb_entries[i].virt_page),PAGE_SIZE,PROT_READ)==-1) {
				trace(DEBUG_WARNING,"failed to protect memory at %p\n",vpage_to_addr(tlb_entries[i].virt_page));
			}
			/* copy data from virtual page to the physical page */
			memcpy(ppage_to_addr(tlb_entries[i].phys_page), vpage_to_addr(tlb_entries[i].virt_page), PAGE_SIZE);
			/* protect this vm page */
			if(mprotect(vpage_to_addr(tlb_entries[i].virt_page),PAGE_SIZE,PROT_NONE)==-1) {
				trace(DEBUG_WARNING,"failed to protect memory at %p\n",vpage_to_addr(tlb_entries[i].virt_page));
			}
		}
	}
}
