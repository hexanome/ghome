/* ------------------------------
   $Id: hw_tools.c 102 2009-11-03 13:14:39Z simon_duquennoy $
   ------------------------------------------------------------

   hw_tools.c

   Module "boite à outils" pour les modules du simulation du matériel.

*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <sched.h>

#include "hardware.h"
#include "hw_tools.h"
#include "hw_mmu.h"

/**
 * table des handlers de ticks élémentaires de simulation
 */
struct sTickHandler {
    int used;
    int rate;
    int next;
    funcHandler callback;
};
#define MAXTICKHANDLERS 16
static struct sTickHandler tickHandler[MAXTICKHANDLERS];

int  Debug		= 0;
char *BaseOfSharedMem	= NULL;
char *TopOfSharedMem	= NULL;
int  *pSigKey		= NULL;
int  main_pid		= 0;

/**
 * Fonction de trace de debuggage
 */
void
trace(int type, const char *msg, ...)
{
    static int isANewLine = 1;
    int i;
    char buffer[2048];
    va_list args;
    buffer[0] = '\0';

    if ((Debug & type) != type)
	return;
	
    if (isANewLine) {
	isANewLine=0;
	fprintf(stderr,"<");
	if ((Debug & DEBUG_WARNING)) {
	    if ((type & DEBUG_WARNING))
		fprintf(stderr,"W");
	    else
		fprintf(stderr,"-");
	}
	if ((Debug & DEBUG_SETUP)) {
	    if ((type & DEBUG_SETUP))
		fprintf(stderr,"S");
	    else
		fprintf(stderr,"-");
	}
	if ((Debug & DEBUG_HWREG)) {
	    if ((type & DEBUG_HWREG))
		fprintf(stderr,"R");
	    else
		fprintf(stderr,"-");
	}
	if ((Debug & DEBUG_IT)) {
	    if ((type & DEBUG_IT))
		fprintf(stderr,"I");
	    else
		fprintf(stderr,"-");
	}
	fprintf(stderr,">");
    }

    va_start(args,msg);
    vsprintf(buffer+strlen(buffer),msg,args);
    va_end(args);

    for (i=0; i<2048 && buffer[i]!='\0'; i++) {
	if (buffer[i]=='\n') {
	    isANewLine = 1;
	    break;
	}
    }
    fprintf(stderr,buffer);
    fflush(stderr);
}

/**
 * IRQ Managment...
 */
static volatile int currentIRQMask = 16;
static volatile int lastIRQCalled = 0;
static int
isIRQHiden(int IRQLevel)
{
    if (IRQLevel < currentIRQMask)
	return 1;

    lastIRQCalled = IRQLevel;
    return 0;
}

int
processUserIRQ(int IRQLevel)
{
	trace(DEBUG_IT,"IRQ (= %d) ",IRQLevel) ;
	if(!isIRQHiden(IRQLevel))
	{
		enum cpu_mode_e prevMode = currentMode;
		trace(DEBUG_IT,"called...\n") ;
		switchToMode(MASTER_MODE);
		IRQVECTOR[IRQLevel]();
		switchToMode(prevMode);
		trace(DEBUG_IT,"returned...\n") ;
		return 1;
	}
	else {
		trace(DEBUG_IT,"canceled...\n") ;
		return 0;
	}
}

void
setIRQMask(int IRQLevel)
{
    currentIRQMask = IRQLevel;
}

/* CPU mode management */
void switchToMode(enum cpu_mode_e mode) {
	if(currentMode != mode) {
		currentMode = mode;
		if(mode == MASTER_MODE) {
			mmuSwitchToMasterMode();
		} else {
			mmuSwitchToUserMode();
		}
	}
}

int checkMasterMode() {
	if(currentMode == USER_MODE) {
		trace(DEBUG_WARNING,"Unauthorized action done in userland\n");
		/* we launch a segfault */
		*(char*)0 = 0;
	}
	return currentMode == MASTER_MODE;
}

void
waitIRQ(int IRQLevel)
{
    int t = currentIRQMask;
    currentIRQMask = 0;
    lastIRQCalled = 0;
    while (lastIRQCalled < IRQLevel) {
	pause();
    }
    currentIRQMask = t;
}

/**
 * ajouter une fonction à appeler toutes les "rate" SYSTICKS
 */
int
addTickHandler(int next, int rate, funcHandler callback)
{
    int i;
    for (i=0; i<MAXTICKHANDLERS; i++) {
	if (!tickHandler[i].used) {
	    tickHandler[i].rate = rate;
	    tickHandler[i].next = next;
	    tickHandler[i].callback = callback;
	    tickHandler[i].used = 1;
	    return 1;
	}
    }
    return 0;
}

/**
 * retirer une fonction de la liste toutes les "rate" micro-seconde
 */
int
removeTicksHandler(funcHandler callback)
{
    int i;
    for (i=0; i<MAXTICKHANDLERS; i++) {
	if ((tickHandler[i].used) && (tickHandler[i].callback == callback)) {
	    tickHandler[i].used = 0;
	    return 1;
	}
    }
    return 0;
}

/**
 * Handle d'interruption pour le SIGALARM
 */
void
signalAlarmHandler()
{
    int i;
    for (i=0; i<MAXTICKHANDLERS; i++) {
	if (tickHandler[i].used) {
	    tickHandler[i].next--;
	    if (!tickHandler[i].next) {
		funcHandler aCallback;
		aCallback = tickHandler[i].callback;
		if (tickHandler[i].rate)
		    tickHandler[i].next += tickHandler[i].rate;
		else
		    removeTicksHandler(tickHandler[i].callback);
		aCallback();
	    }
	}
    }
}

/**
 * table des handlers de signaux élémentaires de simulation
 */
struct sSigHandler {
    int used;
    int key;
    funcSigHandler callback;
};
#define MAXSIGHANDLERS 16
static struct sSigHandler sigHandler[MAXSIGHANDLERS];

/**
 * Ajouter une fonction de la liste des SigHandlers
 */
int
addSigHandler(int key, funcSigHandler callback)
{
    int i;
    for (i=0; i<MAXSIGHANDLERS; i++) {
	if (!sigHandler[i].used) {
	    sigHandler[i].key = key;
	    sigHandler[i].callback = callback;
	    sigHandler[i].used = 1;
	    return 1;
	}
    }
    return 0;
}

/**
 * Retirer une fonction de la liste des SigHandlers
 */
int
removeSigHandler(int key)
{
    int i;
    for (i=0; i<MAXSIGHANDLERS; i++) {
	if ((sigHandler[i].used) && (sigHandler[i].key == key)) {
	    sigHandler[i].used = 0;
	    return 1;
	}
    }
    return 0;
}


/**
 * Handle d'interruption pour le SIGUSR1
 */
void
signalSigHandler(int v)
{
    int i;
    signal(SIGUSR1, signalSigHandler);
    for (i=0; i<MAXSIGHANDLERS; i++) {
	if (sigHandler[i].used && (sigHandler[i].key == *pSigKey))
	    sigHandler[i].callback(*pSigKey);
    }
    *pSigKey = 0;
    sched_yield();
}

/**
 * Générer un signal du type sigHandler (vers le processus père)
 */
int sendSignal(int key)
{
    int ppid = getppid();
    if (main_pid == getpid()) {
	int t = *pSigKey ;
	*pSigKey = key;
	signalSigHandler(0);
	*pSigKey = t;
	return 1;
    } else if ((ppid != 1)) {
	/* wait execution of another signal */
	while ((*pSigKey!=0) && (getppid()!=1))
	    sched_yield();
	*pSigKey = key;
	kill(main_pid,SIGUSR1);
	/* wait execution of the current signal */
	while ((*pSigKey==key) && (getppid()!=1))
	    sched_yield();
	return 1;
    } else
	return 0;
}

/**
 * Get a limited amouth of shared memory
 */
void
*getSharedMem(int size)
{
    void *addr;
    if (TopOfSharedMem < BaseOfSharedMem+SHAREMEMSIZE) {
	addr = (void *)TopOfSharedMem;
	TopOfSharedMem += size;
	return addr;
    }
    return NULL;
}

/**
 * initialisation des outils de simulation matériel
 * (appelée par init_hardware dans hardware.c uniquement...
 */
int
init_Tools()
{
    int shmem_id;
    int i;

    main_pid = getpid();

    /* make a shared mem space */
    shmem_id = shmget(IPC_PRIVATE, SHAREMEMSIZE, IPC_CREAT | 0600);
    if (shmem_id==-1) {
	trace(DEBUG_WARNING,
	      "Hardware_Tools initialization failed, "
	      "cannot open shared memory (EC1)\n");
	return 0;
    }
    trace(DEBUG_SETUP, "Shared memory segement %d intitialized\n", shmem_id);
    
    BaseOfSharedMem = shmat(shmem_id,NULL,0);
    if (((int)TopOfSharedMem) == -1) {
	trace(DEBUG_WARNING,
	      "Hardware_Tools initialization failed, "
	      "cannot open shared memory (EC2)\n");
	BaseOfSharedMem = NULL;
	return 0;
    }
    TopOfSharedMem = BaseOfSharedMem;
    
    /* enregistrement de la destruction du segment memoire apres le
       dernier detachement */  
    shmctl(shmem_id, IPC_RMID, (struct shmid_ds*)0);

    /* preset the ticks handler table */
    for (i=0; i<MAXTICKHANDLERS; i++)
	tickHandler[i].used = 0;

    {
	static struct sigaction new_action;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_handler = signalAlarmHandler;
	new_action.sa_flags = SA_RESTART | SA_NODEFER ;
	sigaction(SIGALRM, &new_action, NULL);
    }
    {
	struct itimerval itimervalue;
	itimervalue.it_interval.tv_sec = 0;
	itimervalue.it_interval.tv_usec = SYSTICKDURATION;
	itimervalue.it_value = itimervalue.it_interval;
	setitimer(ITIMER_REAL, &itimervalue, NULL);
    }

    /* preset the sig handler table */
    for (i=0; i<MAXSIGHANDLERS; i++)
	sigHandler[i].used = 0;
    signal(SIGUSR1, signalSigHandler);

    if ((pSigKey = getSharedMem(sizeof(int)))==NULL) {
	trace(DEBUG_WARNING,
	      "Hardware_Tools initialization failed, "
	      "shared memory alloc failed (EC3)\n");
    }

    /* catch */
    return 1;
}
