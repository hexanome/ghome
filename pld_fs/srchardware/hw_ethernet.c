/* ------------------------------
   $Id: hw_ethernet.c 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_ethernet.c

   Module de simulation d'une carte réseau de type Ethernet. 

*/

#include <stdlib.h>
#include "hardware.h"
#include "hw_tools.h"
#include "hw_ethernet.h"

/**
 * Constantes spécifiants les registres de configuration du matériel simulé.
 * (initialisée en fonction du fichier de configuration)
 */
int   ENABLE_ETHERNET = 0 ;
int   Eth0_Link       = 0 ;
int   Eth0_DMASR      = 0 ;
int   Eth0_DMABASE    = 0 ;
int   Eth0_DMASIZE    = 0 ;
int   Eth0_IRQ        = 0 ;
char *Eth0_MCADR      = NULL ;
int   Eth0_UDPPORT    = 0 ;

/**
 * variables globales d'émulation des liaisons series simulées
 */
int ds_eth0 = 0;

int init_Ethernet()
{
  trace(DEBUG_SETUP,"trying Ethernet initialization...\n");
  trace(DEBUG_SETUP,"Ethernet initialization completed.\n");
	return 1;
}

