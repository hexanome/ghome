/* ------------------------------
   $Id: hw_disk.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_mmu.c 

   Module de gestion de la MMU

*/

#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <stdint.h>
#include "hardware.h"
#include "hw_tools.h"
#include "hw_mmu.h"
#include "hw_mmu_tlb.h"

/* Registers */
int MMU_CMD;		/* Command register */
int MMU_FAULT_ADDR;  /* (read) Last Faulting address catched */

/* Consts */
int MMU_ENABLE;		/* Is MMU simulated? */
int MMU_IRQ; /* ID of the MMU IRQ */

/* globals */
static struct sigaction old_handler, new_handler;
static void *physical_memory_root;
static void *virtual_memory_root;
void *mmu_faulting_addr;
void *physical_memory;
void *virtual_memory;

/* internal SIGSEGV handler for memory faults */
static void h_sigsegv(int sig, siginfo_t *sig_info,void *context) {
/*	switch(sig_info->si_code){
		case SEGV_MAPERR:
			trace(DEBUG_WARNING, "-> illegal memory access\n");
			exit(1);
			break;

		case SEGV_ACCERR:
			if(vm_contains(sig_info->si_addr)) {
				if(currentMode == USER_MODE) {
					mmu_faulting_addr = sig_info->si_addr;
					processUserIRQ(MMU_IRQ);
				} else {
					trace(DEBUG_WARNING, "-> illegal access to the virtual memory\n");
					exit(1);
				}
			} else if(pm_contains(sig_info->si_addr)){
				trace(DEBUG_WARNING, "-> illegal access to the physical memory\n");
				exit(1);
			} else {
				trace(DEBUG_WARNING, "-> illegal memory access\n");
				exit(1);
			}
			break;
	}*/
	mmu_faulting_addr = sig_info->si_addr;
	if(!processUserIRQ(MMU_IRQ)) {
		trace(DEBUG_WARNING, "sigsegv received while MMU_IRQ was masked\n");
		exit(1);
	}
}

/* replace original SIGSEGV handler to catch memory accesses */
static int set_sigsegv_handler(){
	trace(DEBUG_SETUP,"enable MMU\n");
	new_handler.sa_sigaction = h_sigsegv;
	new_handler.sa_flags=SA_SIGINFO|SA_RESTART;	
	if(sigaction(SIGSEGV,&new_handler,&old_handler)==-1){
		trace(DEBUG_WARNING,"failed to set handler for SIGSEGV");
		return 0;
	}
	return 1;
}

/* restore original SIGSEGV handler to catch memory accesses */
static int unset_sigsegv_handler(){
	trace(DEBUG_SETUP,"disable MMU\n");
	if(sigaction(SIGSEGV,&old_handler,0)==-1){
		trace(DEBUG_WARNING,"failed to unset handler for SIGSEGV");
		return 0;
	}
	return 1;
}

/* MMU initialization */
int mmu_init() {
	/* test if mmu is enabled (in hardware.ini)*/
	if (MMU_ENABLE == 0) {
		physical_memory = NULL;
		trace(DEBUG_SETUP,"MMU not supported.\n");
		return 1;
	}

	/* init */
	trace(DEBUG_SETUP,"MMU initialization...\n");

	TLB_ENTRIES_MAX = TLB_ENTRIES + TLB_SIZE;
	
	srand(time(NULL));
	
	/* physical and virtual memory allocation */
	physical_memory_root = malloc(2 * PM_SIZE);
	if(!physical_memory_root) {
		trace(DEBUG_SETUP,"failed to allocate physical memory for MMU\n");
		return 0;
	}
	physical_memory = (void*)(((uintptr_t)physical_memory_root + PM_SIZE) & ~0xfffff);

	if(mprotect((void*)(((uintptr_t)physical_memory_root + 4096) & ~0xfff),2*PM_SIZE,PROT_NONE)==-1) {
		trace(DEBUG_SETUP,"failed to protect physical memory root\n");
		return 0;
	}

	if(mprotect(physical_memory,PM_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC)==-1) {
		trace(DEBUG_SETUP,"failed to protect physical memory\n");
		return 0;
	}
		
	virtual_memory_root = malloc(2 * VM_SIZE + 4096);
	if(!virtual_memory_root) {
		trace(DEBUG_SETUP,"failed to allocate virtual memory root\n");
		return 0;
	}
	virtual_memory = (void*)(((uintptr_t)virtual_memory_root + VM_SIZE) & ~0xffffff);

	if(mprotect((void*)(((uintptr_t)virtual_memory_root + 4096) & ~0xfff),2*VM_SIZE,PROT_NONE)==-1) {
		trace(DEBUG_SETUP,"failed to protect virtual memory\n");
		return 0;
	}
	
	tlb_init();
	
	set_sigsegv_handler();
	
	trace(DEBUG_SETUP,"MMU initialization completed.\n");

	return 1;
}

/* tests if the address is in the virtual memory address space */
char vm_contains(void *addr) {
	return addr >= virtual_memory && (char*)addr < ((char*)virtual_memory + VM_SIZE);
}

/* tests if the address is in the physical memory address space */
char pm_contains(void *addr) {
	return addr >= physical_memory && (char*)addr < ((char*)physical_memory + PM_SIZE);
}


/* Get result of mmu process associating */
int in_MMU_FAULT_ADDR(int port) {
	return (int)mmu_faulting_addr;
}

/* MMU CMD */
void out_MMU_CMD(int port, int value) {
	static int mmu_off = 0;

	if(value == MMU_RESET) {
		trace(DEBUG_SETUP,"reset MMU\n");
		tlb_reset();
	} else if(value == MMU_PROCESS) {
		if(mmu_off) {
			set_sigsegv_handler();
		} else {
			unset_sigsegv_handler();
		}
		mmu_off =! mmu_off;	
	}
}

/* close the MMU */
int mmu_close(){
	tlb_close();
	free(physical_memory_root);
	free(virtual_memory_root);
	return unset_sigsegv_handler();
}
