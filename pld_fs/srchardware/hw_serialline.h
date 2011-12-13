/* ------------------------------
   $Id: hw_serialline.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_serialline.h

   Interface du module de gestion du simulateur de TIMER materiel. 

*/

#ifndef _HW_SERIALLINE_H_
#define _HW_SERIALLINE_H_

/**
 * Constantes spécifiants les registres de configuration du matériel simulé.
 * (initialisée en fonction du fichier de configuration)
 */
extern int  SL1_ENABLE ;
extern char *SL1 ;          
extern int  SL1_COM ;
extern int  SL1_NOISE ;
extern int  SL1_IRQ ;
extern int  SL1_UARTSR ;
extern int  SL1_UARTDATA ;
extern int  SL1_UDPPORT ;
extern int  SL2_ENABLE;
extern char *SL2 ;
extern int  SL2_COM ;
extern int  SL2_NOISE ;
extern int  SL2_IRQ ;
extern int  SL2_UARTSR ;
extern int  SL2_UARTDATA ;
extern int  SL2_UDPPORT ;

/**
 * Initialisation du système de simulation du matériel. 
 */
extern int  init_Serialline();

/**
 * Gestion des entrées/sorties associées à ce matériel.
 */
extern int  in_SL1_UARTSR   (int port) ;
extern int  in_SL1_UARTDATA (int port) ;
extern int  in_TIMER_ALARM  (int port) ;
extern void out_SL1_UARTSR  (int port, int value) ;
extern void out_SL1_UARTDATA(int port, int value) ;
extern void out_TIMER_ALARM(int port, int value) ;

#endif
