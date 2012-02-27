/* ------------------------------
   $Id: hw_ethernet.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_ethernet.h 

   Interface du module de gestion du simulateur de carte Ethernet. 

*/

#ifndef _HW_ETHERNET_H_
#define _HW_ETHERNET_H_

extern int   Eth0_Link ;
extern int   Eth0_DMASR ;
extern int   Eth0_DMABASE ;
extern int   Eth0_DMASIZE ;
extern int   Eth0_IRQ ;
extern char *Eth0_MCADR ;
extern int   Eth0_UDPPORT ;
extern int   ENABLE_ETHERNET;

/**
 * Initialisation du système de simulation du matériel. 
 */
extern int init_Ethernet();

/**
 * Gestion des entrées/sorties associées à ce matériel.
 */

#endif
