/* ------------------------------
   $Id: hw_disk.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   hw_disk.h

   Interface du module de gestion du simulateur de disque dur. 

*/

#ifndef _HW_DISK_H_
#define _HW_DISK_H_

extern int   ENABLE_HDA ;
extern char  *HDA_FILENAME ;
extern int   HDA_CMDREG ;
extern int   HDA_DATAREGS ;
extern int   HDA_MAXDTREGS ;
extern int   HDA_IRQ ;
extern int   HDA_MAXCYLINDER ;
extern int   HDA_MAXSECTOR ;
extern int   HDA_SECTORSIZE ;
extern int   HDA_STPS ;
extern int   HDA_STPC ;
extern int   HDA_PON_DELAY ;
extern int   HDA_POFF_DELAY ;
extern int   ENABLE_HDB ;
extern char  *HDB_FILENAME ;
extern int	 HDB_CMDREG ;
extern int   HDB_DATAREGS ;
extern int   HDB_MAXDTREGS ;
extern int   HDB_IRQ ;
extern int   HDB_MAXCYLINDER ;
extern int   HDB_MAXSECTOR ;
extern int   HDB_SECTORSIZE ;
extern int   HDB_STPC ;
extern int   HDB_STPS ;
extern int   HDB_PON_DELAY ;
extern int   HDB_POFF_DELAY ;

/**
 * Initialisation du système de simulation du matériel. 
 */
extern int   init_Disk();

/**
 * Gestion des entrées/sorties associées à ce matériel.
 */
extern int  in_HDA_DATAREGS (int port) ;
extern int  in_HDA_CMDREG   (int port) ;
extern int  in_HDB_DATAREGS (int port) ;
extern int  in_HDB_CMDREG   (int port) ;
extern void out_HDA_DATAREGS(int port, int value) ;
extern void out_HDA_CMDREG  (int port, int value) ;
extern void out_HDB_DATAREGS(int port, int value) ;
extern void out_HDB_CMDREG  (int port, int value) ;

#endif 

