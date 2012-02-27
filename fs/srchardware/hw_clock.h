/* ------------------------------
   $Id: hw_clock.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_clock.h

   Interface du module de gestion du simulateur de TIMER materiel. 

*/

#ifndef _HW_CLOCK_H_
#define _HW_CLOCK_H_

/**
 * Constantes sp�cifiants les registres de configuration du mat�riel simul�.
 * (initialis�e en fonction du fichier de configuration)
 */
extern int   TIMER_CLOCK ;  /* n� registre associ� au registre TIMER_CLOCK */
extern int   TIMER_PARAM ;  /* n� registre associ� au registre TIMER_PARAM */
extern int   TIMER_ALARM ;  /* n� registre associ� au registre TIMER_ALARM */
extern int   TIMER_IRQ ;    /* niveau d'interruption associ� (d�finit par TIMER_IRQ) */
extern int   TIMER_TICKS ;

/**
 * Initialisation du syst�me de simulation du mat�riel. 
 */
extern int  init_Clock();

/**
 * Gestion des entr�es/sorties associ�es � ce mat�riel.
 */
extern int  in_TIMER_CLOCK (int port) ;
extern int  in_TIMER_PARAM (int port) ;
extern int  in_TIMER_ALARM (int port) ;
extern void out_TIMER_CLOCK(int port, int value) ;
extern void out_TIMER_PARAM(int port, int value) ;
extern void out_TIMER_ALARM(int port, int value) ;



#endif
