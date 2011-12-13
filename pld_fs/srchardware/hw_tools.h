/* ------------------------------
   $Id: hw_tools.h 114 2009-12-01 13:06:43Z simon_duquennoy $
   ------------------------------------------------------------

   hw_tools.h

   Interface du module "boite � outils" pour les modules de simulation
   du materiel.   

*/

#ifndef _HW_TOOLS_H_
#define _HW_TOOLS_H_

/**
 * Modes du CPU
 */
enum cpu_mode_e{MASTER_MODE, USER_MODE};
extern enum cpu_mode_e currentMode;

/**
 * initialisation des outils de simulation mat�riel
 * (appel�e par init_hardware dans hardware.c uniquement...
 */
extern int init_Tools();

/**
 * Taille de la m�moire partag�e disponible
 */
#define SHAREMEMSIZE  512
/**
 * Constantes de trace de Buggage
 */
#define DEBUG_SETUP   0x0001
#define DEBUG_IT      0x0010
#define DEBUG_HWREG   0x0100
#define DEBUG_WARNING 0x1000
/**
 * variable associ�e au drapeau de debug actifs
 */
extern int   Debug;

/**
 *
 */
typedef void (*funcHandler)();
typedef void (*funcSigHandler)(int param);

/**
 * Fonction de trace de debuggage
 */
void trace(int type, const char *msg, ...) ;

/**
 * Fonctions d'aide � la gestion des interruptions
 */
void setIRQMask(int IRQLevel);
void waitIRQ(int IRQLevel);

/**
 * Fonctions de gestion du mode CPU
 */
extern void switchToMode(enum cpu_mode_e mode);
extern int checkMasterMode();

/**
 * appeler un handler d'IRQ
 */
int processUserIRQ(int IRQLevel);

/**
 * ajouter une fonction � appeler toutes les "rate" micro-seconde
 */
int addTickHandler(int next, int rate, funcHandler callback);
/**
 * retirer une fonction de la liste toutes les "rate" micro-seconde
 */
int removeTicksHandler(funcHandler callback);

/**
 * ajouter une fonction � appeler sur SIGUSR1 + key
 */
int addSigHandler(int key, funcSigHandler callback) ;

/**
 * retirer une fonction de la liste toutes les "rate" micro-seconde
 */
int removeSigHandler(int key) ;

/**
 * G�n�rer un signal du type sigHandler (vers le processus p�re)
 */
int sendSignal(int key) ;

/**
 * Allouer une zone de m�moire partag�e de taille Size
 */
void *getSharedMem(int size) ;

#endif 
