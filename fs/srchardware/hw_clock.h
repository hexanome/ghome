/* ------------------------------
   $Id: hw_clock.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_clock.h

   Interface du module de gestion du simulateur de TIMER materiel. 

*/

#ifndef _HW_CLOCK_H_
#define _HW_CLOCK_H_

/**
 * Constantes spécifiants les registres de configuration du matériel simulé.
 * (initialisée en fonction du fichier de configuration)
 */
extern int   TIMER_CLOCK ;  /* n° registre associé au registre TIMER_CLOCK */
extern int   TIMER_PARAM ;  /* n° registre associé au registre TIMER_PARAM */
extern int   TIMER_ALARM ;  /* n° registre associé au registre TIMER_ALARM */
extern int   TIMER_IRQ ;    /* niveau d'interruption associé (définit par TIMER_IRQ) */
extern int   TIMER_TICKS ;

/**
 * Initialisation du système de simulation du matériel. 
 */
extern int  init_Clock();

/**
 * Gestion des entrées/sorties associées à ce matériel.
 */
extern int  in_TIMER_CLOCK (int port) ;
extern int  in_TIMER_PARAM (int port) ;
extern int  in_TIMER_ALARM (int port) ;
extern void out_TIMER_CLOCK(int port, int value) ;
extern void out_TIMER_PARAM(int port, int value) ;
extern void out_TIMER_ALARM(int port, int value) ;



#endif
