/* ------------------------------
   $Id: hardware.c 114 2009-12-01 13:06:43Z simon_duquennoy $
   ------------------------------------------------------------

    hardware.c 

    Bibliothèque de simulation du matériel. 
    voir hardware.ps pour le detail du matériel simulé par cette bibliothèque. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "internals.h"

#include "hardware.h"
#include "hw_tools.h"
#include "hw_clock.h"
#include "hw_serialline.h"
#include "hw_ethernet.h"
#include "hw_disk.h"
#include "hw_mmu.h"
#include "hw_mmu_tlb.h"

/* CPU mode */
enum cpu_mode_e currentMode = MASTER_MODE;

/**
 * Constantes et variables de gestion du matériel :
 */
unsigned char *MemBase;
func_irq      *irq_vector;
func_irq      global_irq_vector[IRQ_VECOTR_SIZE];
int           SYSTICKDURATION;

/**
 * état d'initialisation du matériel
 */
static int  libinit = 0;


/**
 * table des constantes de configuration
 */

typedef struct _cstConfig {
	char *name;    /* nom symbolique de la constante */
	char type;     /* type de la constante ('I' : entier, 'S': chaine de caractère) */
	union {
		int *integer;		/* adresse de la variable int à mettre à jour */
		char **string;	    /* adresse de la variable char * à mettre à jour */
	} address;
} cstConfig;

static cstConfig cstConfigTable[] = 
{
    {"DEBUG"          ,'I' ,{&Debug               } } ,
    {"SL1_ENABLE"     	,'I' ,{&SL1_ENABLE          } } ,
    {"SL1"            ,'S' ,{(int *)&SL1          } } ,
    {"SL1_COM"        ,'I' ,{&SL1_COM             } } ,
    {"SL1_NOISE"      	,'I' ,{&SL1_NOISE           } } ,
    {"SL1_IRQ"        ,'I' ,{&SL1_IRQ             } } ,
    {"SL1_UARTSR"     ,'I' ,{&SL1_UARTSR          } } ,
    {"SL1_UARTDATA"   ,'I' ,{&SL1_UARTDATA        } } ,
    {"SL1_UDPPORT"    ,'I' ,{&SL1_UDPPORT         } } ,
    {"SL2_ENABLE"     	,'I' ,{&SL2_ENABLE          } } ,
    {"SL2"            ,'S' ,{(int *)&SL2          } } ,
    {"SL2_COM"        ,'I' ,{&SL2_COM             } } ,
    {"SL2_NOISE"      ,'I' ,{&SL2_NOISE           } } ,
    {"SL2_IRQ"       	 ,'I' ,{&SL2_IRQ             } } ,
    {"SL2_UARTSR"     ,'I' ,{&SL2_UARTSR          } } ,
    {"SL2_UARTDATA"   ,'I' ,{&SL2_UARTDATA        } } ,
    {"SL2_UDPPORT"    ,'I' ,{&SL2_UDPPORT         } } ,
    {"ENABLE_ETHERNET"	,'I' ,{&ENABLE_ETHERNET     } } ,
    {"Eth0_Link"      ,'I' ,{&Eth0_Link           } } ,
    {"Eth0_DMASR"     ,'I' ,{&Eth0_DMASR          } } ,
    {"Eth0_DMABASE"   ,'I' ,{&Eth0_DMABASE        } } ,
    {"Eth0_DMASIZE"   ,'I' ,{&Eth0_DMASIZE        } } ,
    {"Eth0_IRQ"       	,'I' ,{&Eth0_IRQ            } } ,
    {"Eth0_MCADR"     ,'S' ,{(int *)&Eth0_MCADR   } } ,
    {"Eth0_UDPPORT"   ,'I' ,{&Eth0_UDPPORT        } } ,
    {"ENABLE_HDA"     ,'I' ,{&ENABLE_HDA          } } ,
    {"HDA_FILENAME"   	,'S' ,{(int *)&HDA_FILENAME } } ,
    {"HDA_CMDREG"     ,'I' ,{&HDA_CMDREG          } } ,
    {"HDA_DATAREGS"   ,'I' ,{&HDA_DATAREGS        } } ,
    {"HDA_IRQ"        ,'I' ,{&HDA_IRQ             } } ,
    {"HDA_MAXSECTOR"  ,'I' ,{&HDA_MAXSECTOR       } } ,
    {"HDA_MAXCYLINDER",'I' ,{&HDA_MAXCYLINDER     } } ,
    {"HDA_SECTORSIZE" ,'I' ,{&HDA_SECTORSIZE      } } ,
    {"HDA_STPS"       ,'I' ,{&HDA_STPS            } } ,
    {"HDA_STPC"       ,'I' ,{&HDA_STPC            } } ,
    {"HDA_PON_DELAY"  ,'I' ,{&HDA_PON_DELAY       } } ,
    {"HDA_POFF_DELAY" ,'I' ,{&HDA_POFF_DELAY      } } ,
    {"ENABLE_HDB"     ,'I' ,{&ENABLE_HDB          } } ,
    {"HDB_FILENAME"   ,'S' ,{(int *)&HDB_FILENAME } } ,
    {"HDB_CMDREG"     ,'I' ,{&HDB_CMDREG          } } ,
    {"HDB_DATAREGS"   ,'I' ,{&HDB_DATAREGS        } } ,
    {"HDB_IRQ"        ,'I' ,{&HDB_IRQ             } } ,
    {"HDB_MAXSECTOR"  	,'I' ,{&HDB_MAXSECTOR       } } ,
    {"HDB_MAXCYLINDER"	,'I' ,{&HDB_MAXCYLINDER     } } ,
    {"HDB_SECTORSIZE" 	,'I' ,{&HDB_SECTORSIZE      } } ,
    {"HDB_STPS"       	,'I' ,{&HDB_STPS            } } ,
    {"HDB_STPC"       	,'I' ,{&HDB_STPC            } } ,
    {"HDB_PON_DELAY"  	,'I' ,{&HDB_PON_DELAY       } } ,
    {"HDB_POFF_DELAY" 	,'I' ,{&HDB_POFF_DELAY      } } ,
    {"TIMER_CLOCK"    	,'I' ,{&TIMER_CLOCK         } } ,
    {"TIMER_PARAM"    	,'I' ,{&TIMER_PARAM         } } ,
    {"TIMER_ALARM"    	,'I' ,{&TIMER_ALARM         } } ,
    {"TIMER_IRQ"      	,'I' ,{&TIMER_IRQ           } } ,
    {"TIMER_TICKS"    	,'I' ,{&TIMER_TICKS	  } } ,
    {"SYSTICK"        	,'I' ,{&SYSTICKDURATION     } } ,
    {"MMU_ENABLE"        	,'I' ,{&MMU_ENABLE	    } } ,
    {"MMU_IRQ"        	,'I' ,{&MMU_IRQ	    } } ,
    {"MMU_CMD"        	,'I' ,{&MMU_CMD     } } ,
    {"MMU_FAULT_ADDR",'I' ,{&MMU_FAULT_ADDR        } } ,	
    {"TLB_ADD_ENTRY",'I' ,{&TLB_ADD_ENTRY        } } ,
    {"TLB_DEL_ENTRY",'I' ,{&TLB_DEL_ENTRY        } } ,
    {"TLB_SIZE",'I' ,{&TLB_SIZE        } } ,
    {"TLB_ENTRIES",'I' ,{&TLB_ENTRIES        } } ,
    {NULL             ,'-' ,{ NULL                } } /* derniere entrée */
};

/**
 * Fonction de mise à jour d'une constante de configuration entiere. 
 */
int setIValue(char *name, int  v)
{
    int i = 0;
    while (cstConfigTable[i].name) {
	if (strcmp(cstConfigTable[i].name,name)==0) {
	    if(cstConfigTable[i].type == 'I') {
		*cstConfigTable[i].address.integer = v ;
		trace(DEBUG_SETUP," % 20s = %d (0x%X) \n",name,v,v);
		return 1;
	    } else {
		trace(DEBUG_WARNING,
		      "Warning constant %s must be an Integer value \n",name);
		return 1;
	    }
	}
	i++;
    }
    trace(DEBUG_WARNING,"Warning defined constant %s is unknown \n",name);
    return 0;
}

/**
 * Fonction de mise à jour d'une constante de configuration "chaine de charactères". 
 */
int setSValue(char *name, char *v)
{
    char *s;
    int i = 0;
    while (cstConfigTable[i].name) {
	if(strcmp(cstConfigTable[i].name,name)==0) {
	    if(cstConfigTable[i].type == 'S') {
		s = malloc(strlen(v)+1);
		strcpy(s,v);
		*cstConfigTable[i].address.string = s ;
		trace(DEBUG_SETUP," % 20s = \"%s\" \n",name,v);
		return 0;
	    } else {
		trace(DEBUG_WARNING,
		      "Warning constant %s must be a String value \n",name);
		return 0;
	    }
	}
	i++;
    }
    trace(DEBUG_WARNING,"Warning defined constant %s is unknown \n",name);
    return 0;
}


/**
 *      int init_hardware();
 *          initialisation du matériel. Pas de "reinitialisation" possible.  
 *          retourne 0 en cas de problème lors de l'initialisation.
 */ 
extern void yyrestart( FILE *input_file);
extern void yyparse();


void _noIT(void)
{
    trace(DEBUG_WARNING,"An unimplemented IT is called...\n");
    exit(1);
    return ;
}

int     init_hardware(const char *filename)
{
	int i;
    int res = 1;
    FILE *f;
        
    if(libinit != 0)
	return 0;
    
    /* Initialisation des constantes matérielles. */
    if( (f = fopen(filename,"r"))==NULL) {
	trace(DEBUG_WARNING,"file %s not found!\n",filename);
	return 0;
    }
    yyrestart(f);
    yyparse();
    fclose(f);

    /* activate the hardware tools set */    
    if(!init_Tools()) {
	trace(DEBUG_WARNING,"Hardware initialization failed\n");
	return 0;
    }
    /* Initialisation du timer device */
    if(!init_Clock()) {
	trace(DEBUG_WARNING,"Timer initialization failed\n");
	res = 0;
    }
    /* Initialisation des port com */
    if(!init_Serialline()) {
	trace(DEBUG_WARNING,"Serialline initialization failed\n");
	res = 0;
    }
    /* Initialisation de la carte ethernet */
  if(!init_Ethernet()) {
      trace(DEBUG_WARNING,"Ethernet initialization failed\n");
      res = 0;
  }
  /* Initialisation du disque dur. */
  if(!init_Disk()) {
      trace(DEBUG_WARNING,"Disk initialization failed\n");
      res = 0;
  }
    /* Initialisation de la MMU */
  if(!mmu_init()){
      trace(DEBUG_WARNING,"MMU initialization failed\n");
      res = 0;
  }

  /* init IT */
  if(physical_memory == NULL){
      irq_vector = global_irq_vector;
  } else {
      irq_vector = physical_memory;
  }  
  for(i=0 ; i<IRQ_VECOTR_SIZE ; i++) { 
	IRQVECTOR[0]  = _noIT; /* capturer l'interruption */
  }
  libinit = 1;
  return res;
}

/**
 * table des constantes de configuration
 */
typedef int  (funcIn)(int port);
typedef void (funcOut)(int port, int value);

int
in_noop(int port)
{
    trace(DEBUG_WARNING,
	  "Reading the none implemented register nb 0x%X...\n", port);
    return 0;
}

void
out_noop(int port, int value)
{
    trace(DEBUG_WARNING,
	  "Writing the none implemented register nb 0x%X...\n", port);
}

typedef struct _inOutRegister {
    char    *name;       /* nom symbolique du registre */
    int     *regMinId;   /* pointeur sur une valeur qui définit le n° min du registre */
    int     *regMaxId;   /* pointeur sur une valeur qui définit le n° max du registre */
    funcIn  *infct;      /* pointeur sur une fonction "in" qui simule la lecture de ce registre */
    funcOut *outfct;     /* pointeur sur une fonction "out" qui simule la lecture de ce registre */
} inOutRegister;

static inOutRegister inOutRegisterTable[] = 
{
	{"HDA_DATAREGS" 	 ,&HDA_DATAREGS ,&HDA_MAXDTREGS, &in_HDA_DATAREGS , &out_HDA_DATAREGS } ,
	{"HDA_CMDREG"   	 ,&HDA_CMDREG   ,&HDA_CMDREG   , &in_HDA_CMDREG   , &out_HDA_CMDREG   } ,
	{"HDB_DATAREGS" 	 ,&HDB_DATAREGS ,&HDB_MAXDTREGS, &in_HDB_DATAREGS , &out_HDB_DATAREGS } ,
	{"HDB_CMDREG"   	 ,&HDB_CMDREG   ,&HDB_CMDREG   , &in_HDB_CMDREG   , &out_HDB_CMDREG   } ,
	{"SL1_UARTSR"   	 ,&SL1_UARTSR   ,&SL1_UARTSR   , &in_SL1_UARTSR   , &out_SL1_UARTSR   } ,
	{"SL1_UARTDATA" 	 ,&SL1_UARTDATA ,&SL1_UARTDATA , &in_SL1_UARTDATA , &out_SL1_UARTDATA } ,
	{"SL2_UARTSR"   	 ,&SL2_UARTSR   ,&SL2_UARTSR   , &in_noop         , &out_noop         } ,
	{"SL2_UARTDATA" 	 ,&SL2_UARTSR   ,&SL2_UARTSR   , &in_noop         , &out_noop         } ,
	{"Eth0_DMASR"   	 ,&Eth0_DMASR   ,&Eth0_DMASR   , &in_noop         , &out_noop         } ,
	{"Eth0_DMABASE" 	 ,&Eth0_DMABASE ,&Eth0_DMABASE , &in_noop         , &out_noop         } ,
	{"Eth0_DMASIZE" 	 ,&Eth0_DMASIZE ,&Eth0_DMASIZE , &in_noop         , &out_noop         } ,
	{"TIMER_CLOCK"  	 ,&TIMER_CLOCK  ,&TIMER_CLOCK  , &in_TIMER_CLOCK  , &out_TIMER_CLOCK  } ,
	{"TIMER_PARAM"  	 ,&TIMER_PARAM  ,&TIMER_PARAM  , &in_TIMER_PARAM  , &out_TIMER_PARAM  } ,
	{"TIMER_ALARM"  	 ,&TIMER_ALARM  ,&TIMER_ALARM  , &in_TIMER_ALARM  , &out_TIMER_ALARM  } ,
	{"MMU_CMD"  	 	 ,&MMU_CMD      ,&MMU_CMD      , &in_noop         , &out_MMU_CMD  },
	{"MMU_FAULT_ADDR",&MMU_FAULT_ADDR,&MMU_FAULT_ADDR, &in_MMU_FAULT_ADDR , &out_noop } ,
	{"TLB_ADD_ENTRY",     &TLB_ADD_ENTRY,    &TLB_ADD_ENTRY     , &in_noop , &out_TLB_ADD_ENTRY } ,
	{"TLB_DEL_ENTRY",     &TLB_DEL_ENTRY,    &TLB_DEL_ENTRY     , &in_noop , &out_TLB_DEL_ENTRY } ,
	{"TLB_ENTRIES",     &TLB_ENTRIES,    &TLB_ENTRIES_MAX     , &in_TLB_ENTRIES , &out_TLB_ENTRIES } ,
	{NULL           ,NULL        ,NULL        , NULL          , NULL } /* derniere entrée */
};

/**
 *      int _in(int port);
 *          lecture du contenu du registre matériel n° port.
 *          retourne la valeur lue.
 */ 
int     _in(int port)
{
	if(checkMasterMode()) {
		int i = 0;
		while(inOutRegisterTable[i].name != NULL) {
		if( ((*(inOutRegisterTable[i].regMinId)) <= port) &&
			((*(inOutRegisterTable[i].regMaxId)) >= port)) {
			trace(DEBUG_HWREG,"Get %s (0x%X)",inOutRegisterTable[i].name, port);
			i = inOutRegisterTable[i].infct(port) ;
			trace(DEBUG_HWREG," return %d (0x%X) \n",i, i);
			return i;
		}
		i++;
		}
		trace(DEBUG_WARNING,"Reading unknown hardware register 0x%X...\n",port);
	}
	return 0;
}

/**
 *      void _out(int port, int value);
 *          ecriture de la valeur value dans le registre matériel n°port.
 *
 */
void    _out(int port, int value)
{
	if(checkMasterMode()) {
		int i = 0;
		while(inOutRegisterTable[i].name != NULL) {
		if( *(inOutRegisterTable[i].regMinId) <= port )
			if ( *(inOutRegisterTable[i].regMaxId) >= port) {
			trace(DEBUG_HWREG,
				  "Set %s (0x%X) with %d (0x%X). \n",
				  inOutRegisterTable[i].name, port, value,value);
			inOutRegisterTable[i].outfct(port,value) ;
			return ;
			}
		i++;
		}
		trace(DEBUG_WARNING,"Writing unknown hardware register 0x%X...\n",port);
	}
}

/**
 *      void _sleep(int irqLevel);
 *          Stoppe l'activité du microprocesseur jusqu'à l'occurence
 *          une interruption de niveau au moins égale à irqLevel.
 *
 */
void     _sleep(int irqLevel)
{
    waitIRQ(irqLevel);
}

/**
 *      void _mask(int irqLevel);
 *          - cache au microprocesseur l'occurence d'interruptions 
 *          de niveau inférieure à irqLevel.
 * 			- 16ème bit à 0 : passage en mode protégé
 * 			- 16ème bit à 1 : passage en mode user
 *
 */
void     _mask(int irqLevel)
{
	if(checkMasterMode()) {
		setIRQMask(irqLevel & 0xf);
		switchToMode((irqLevel & 0x1000) == 0 ? MASTER_MODE : USER_MODE);
	}
}

/**
 *      void _int(int irqLevel);
 *          - lance une interruption logicielle de niveau irqLevel
 *
 */
void     _int(int irqLevel)
{
	if(irqLevel >= 16 && irqLevel < IRQ_VECOTR_SIZE) {
		processUserIRQ(irqLevel);
	} else {
		trace(DEBUG_WARNING,"Irq %d is not a software interrupt\n", irqLevel);
	}
}
