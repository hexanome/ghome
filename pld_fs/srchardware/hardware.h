/* ------------------------------
   $Id: hardware.h 114 2009-12-01 13:06:43Z simon_duquennoy $
   ------------------------------------------------------------

   hardware.h

   Interface de la bibliothèque de simulation du matériel. 

*/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/**
 * CMD_
 * commandes ATA-2
 */
#define CMD_SEEK        0x02
#define CMD_READ        0x04
#define CMD_WRITE       0x06
#define CMD_FORMAT      0x08
#define CMD_STATUS      0x12
#define CMD_DMASET      0x14
#define CMD_DSKINFO     0x16
#define CMD_MANUF       0xA2
#define CMD_DIAG        0xA4

/**
 * Commandes de la MMU (registre MMU_CMD)
 */
#define MMU_PROCESS	0xCC	/* Commande d'activation/désactivation de la MMU */
#define MMU_RESET	0xD5	/* Commande de réinitialisation de la MMU */

/**
 * Physical and virtual memory for MMU
 */
extern void *physical_memory;
extern void *virtual_memory;

/**
 * prototype des fonctions-interruptions.
 * une interruption ne recoit aucun paramêtre "d'appel", 
 * une interruption ne retourne aucun resultat, mais
 * sa terminaison restaure le contexte d'exécution du programme interrompu. 
 */
typedef void (*func_irq)(void);

/**
 *      int init_hardware(const char *fileconfig);
 *          initialisation du matériel. Pas de "reinitialisation" possible.
 *          l'initialisation définit le matériel conformément aux spécifications
 *          fournies par le fichier dont le nom est "fileconfig".
 *          retourne 0 en cas de problème lors de l'initialisation.
 */ 
extern int init_hardware(const char *fileconfig);

/**
 *      IRQVECTOR 
 *          donne la base d'un tableau de pointeur de fonction du type
 *          func_irq. la fonction IRQVECTOR[n]() est appelée lorsque 
 *          l'interuption de niveau n est déclanchée par le matériel.
 */
#define IRQ_VECOTR_SIZE 256
extern func_irq *irq_vector;	/* n'utilisez pas cette variable    */
#define IRQVECTOR irq_vector	/* préférez ce #define IRQVECTOR    */

extern int SYSTICKDURATION;	/* microseconde entre les SYSTICK   */

/**
 * MASTERBUFFER et SLAVEBUFFER
 *     Adresses des buffers de donnees des disques maitre et esclave
 */
/* n'utilisez pas ces variables*/
extern unsigned char ** HDA_masterbufferaddress, **HDB_masterbufferaddress;
/* préférez ces #define MASTERBUFFER et SLAVEBUFFER */
#define MASTERBUFFER (*HDA_masterbufferaddress) 
#define SLAVEBUFFER  (*HDB_masterbufferaddress) 

/**
 *      BASEADDRESS_RAM
 *          variable associée à adresse de base de la mémoire globale
 *          de la machine. Cette mémoire est commune à tout les programmes 
 *          qui utilisent la librairie sur la même machine.  
 *          
 */
extern  unsigned char *baseGlobalMem;       /* n'utilisez pas cette variable */
#define BASEADDRESS_RAM baseGlobalMem /* préférez ce #define BASEADDRESS_RAM */

/**
 *      int _in(int port);
 *          lecture du contenu du registre matériel n° "port".
 *          retourne la valeur lue.
 */ 
int     _in(int port);

/**
 *      void _out(int port, int value);
 *          ecriture de la valeur "value" dans le registre matériel n° "port".
 */
void    _out(int port, int value);

/**
 *      void _sleep(int irq_level);
 *          Stoppe l'activité du microprocesseur jusqu'à l'occurence
 *          une interruption de niveau au moins égale à "irqLevel".
 */
void    _sleep(int irq_level);

/**
 *      void _mask(int irqLevel);
 *          - cache au microprocesseur l'occurence d'interruptions 
 *          de niveau inférieure à irqLevel.
 * 			- 16ème bit à 0 : passage en mode protégé
 * 			- 16ème bit à 1 : passage en mode user
 *
 */
void     _mask(int irq_level);

/**
 *      void _int(int irqLevel);
 *          - lance une interruption logicielle de niveau irqLevel
 *
 */
void     _int(int irqLevel);

#endif

