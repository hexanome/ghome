/* ------------------------------
   $Id: hw_disk.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_disk.c 

   Module de gestion du simulateur de disque dur. 

*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include "hardware.h"
#include "hw_tools.h"
#include "hw_disk.h"

/**
 * ATA Constants..
 */
/* ATA Command */
#define DSK_SEEK      0x02    /* Shift disk head                   */
#define DSK_READ      0x04    /* Read disk data                    */
#define DSK_WRITE     0x06    /* Write disk data                   */
#define DSK_FORMAT    0x08    /* Format disk sectors               */
#define DSK_GETSTATUS 0x12    /* Get disk status register          */
#define DSK_SETSTATUS 0x14    /* Get disk status register          */
#define DSK_DSKNFO    0x16    /* Get disk geometry informations    */
#define DSK_MANUF     0xA2    /* Get disk manufacturer information */
#define DSK_DIAG      0xA4    /* Make a disk diagnostic            */
/* Disk states */
#define DSKSTATE_POFF 0x00 /* */
#define DSKSTATE_IDLE 0x01 /* */
#define DSKSTATE_SEEK 0x02 /* */
#define DSKSTATE_LOAD 0x03 /* */
#define DSKSTATE_STOR 0x04 /* */
#define DSKSTATE_FRMT 0x05 /* */

/**
 * Constantes spécifiants les registres de configuration du matériel simulé.
 * (initialisée en fonction du fichier de configuration)
 */
int   ENABLE_HDA      = 0;
char  *HDA_FILENAME   = NULL;
int   HDA_CMDREG      = 0;
int   HDA_DATAREGS    = 0;
int   HDA_MAXDTREGS   = 0;
int   HDA_IRQ         = 0;
int   HDA_MAXCYLINDER = 0;
int   HDA_MAXSECTOR   = 0;
int   HDA_SECTORSIZE  = 0;
int   HDA_STPC        = 0;
int   HDA_STPS        = 0;
int   HDA_PON_DELAY   = 0;
int   HDA_POFF_DELAY  = 0;

int   ENABLE_HDB      = 0;
char  *HDB_FILENAME   = NULL;
int   HDB_CMDREG      = 0;
int   HDB_DATAREGS    = 0;
int   HDB_MAXDTREGS   = 0;
int   HDB_IRQ         = 0;
int   HDB_MAXCYLINDER = 0;
int   HDB_MAXSECTOR   = 0;
int   HDB_SECTORSIZE  = 0;
int   HDB_STPS        = 0;
int   HDB_STPC        = 0;
int   HDB_PON_DELAY   = 0;
int   HDB_POFF_DELAY  = 0;

/**
 * variables globales d'émulation des liaisons series simulées
 */

struct dskStatus {
	int enable ;						/* is disk activated ? */
	int state ;							/* current disk state.  See #define DSKSTATE_XXX */
	int goalState ;					/* targeted disk state. See #define DSKSTATE_XXX */
	int nextSectorEvent ;		/* delay before the next sector shift (in SYSTICKS)... */
	int nextCylinderEvent ;	/* delay before the next cylinder shift (in SYSTICKS)... */
	int flags ;							/* */
	int nbop ;							/* general purposed delay counter */
	int delay ;							/* general purposed delay counter */
	volatile int currentSector ;		/* current sector */
	volatile int currentCylinder ;	/* current cylinder */
	int targetSector ;			/* destination sector */
	int targetCylinder ;		/* destination cylinder */
	int MAXCYLINDER ;				/* \                 */
	int MAXSECTOR ;					/*  | disk geometry  */
	int SECTORSIZE ;				/* /                 */
  int STPC ;							/* System ticks per cylinder */
  int STPS ;							/* System ticks per sector */
	int PON_DELAY ;         /* delay before activation */
	int POFF_DELAY ;        /* delay before disk shut down */
	int IRQ ;								/* IRQ level associated with this disk */
	int DATAREGS ;					/* address of the data regs bank */
	int CMDREG ;						/* address of the command regs bank */
	FILE *dskFile ;					/* associated file */
	unsigned char dataRegs[16] ;	/* current data reg value */
	unsigned char cmdReg ;				/* current command reg value */
	unsigned char *buffer ;				/* address of the disk buffer */
} ;

static struct dskStatus HDAStatus;
unsigned char **HDA_masterbufferaddress = &HDAStatus.buffer ;
static struct dskStatus HDBStatus;
unsigned char **HDB_masterbufferaddress = &HDBStatus.buffer ;

/**
 * Initialisation du système de simulation du matériel. 
 */
void HDAHandler();
void HDBHandler();

int init_Disk()
{
	int sector,i;
	HDAStatus.enable = ENABLE_HDA;
	HDBStatus.enable = ENABLE_HDB;
	if ( (ENABLE_HDA==0) && (ENABLE_HDB==0) )
	{
    trace(DEBUG_SETUP,"Disk Hardware not supported.\n");
		return 1;
	}
  trace(DEBUG_SETUP,"trying Disk initialization...\n");
	/* compute address of the last data register */
	HDA_MAXDTREGS = HDA_DATAREGS + 16 ;
	HDB_MAXDTREGS = HDB_DATAREGS + 16 ;
	if (ENABLE_HDA)
	{
		if ( (HDAStatus.dskFile = fopen(HDA_FILENAME,"r+b")) == NULL ) /* read and write mode */
		{
			/* Error while opening the file. File does not exists...
				Create a new one and init it with random values. */
			trace(DEBUG_SETUP,"Trying to build a new HDA support...\n");
			/* Create it */
			if( (HDAStatus.dskFile = fopen(HDA_FILENAME,"wb")) == NULL)
			{
				trace(DEBUG_WARNING,"failed to create HDA File named %s \n",HDA_FILENAME);
				return 0;
			}
			/* build the file header */
			fwrite(&HDA_MAXCYLINDER,sizeof(int),1,HDAStatus.dskFile);
			fwrite(&HDA_MAXSECTOR  ,sizeof(int),1,HDAStatus.dskFile);
			fwrite(&HDA_SECTORSIZE ,sizeof(int),1,HDAStatus.dskFile);
			/* init random seed */
			srand(time(NULL));
			/* for each sector */
			for( sector=0 ; sector<HDA_MAXCYLINDER*HDA_MAXSECTOR ; sector++ )
			{
				for(i=0;i<HDA_SECTORSIZE;i++)
				{
					fputc( rand(), HDAStatus.dskFile);
				}
			}
			fflush(HDAStatus.dskFile);
			fclose(HDAStatus.dskFile);
			if ( (HDAStatus.dskFile = fopen(HDA_FILENAME,"r+b")) == NULL ) /* read and write mode */
			{
				trace(DEBUG_WARNING,"failed to create HDA File named %s \n",HDA_FILENAME);
				return 0;
			}
			trace(DEBUG_SETUP,"HDA creation completed\n");
		}
		else
		{
			int maxsec;
			int maxcyl;
			int sectorSize;
			trace(DEBUG_SETUP,"disk image file \"%s\" open.\n",HDA_FILENAME);
			/* read current HDA MAXCYLINDER */
			fread(&maxcyl,sizeof(int),1,HDAStatus.dskFile);
			fread(&maxsec,sizeof(int),1,HDAStatus.dskFile);
			fread(&sectorSize,sizeof(int),1,HDAStatus.dskFile);
			if( (maxcyl != HDA_MAXCYLINDER) ||
				  (maxsec != HDA_MAXSECTOR) ||
					(sectorSize != HDA_SECTORSIZE) )
			{
				trace(DEBUG_WARNING,"the current \"%s\" file used a wrong geometry(MAXCYLINDER=%d,MAXSECTOR=%d,SECTORSIZE=%d)\n",HDA_FILENAME,maxcyl,maxsec,sectorSize);
				fclose(HDAStatus.dskFile);
				return 0;
			}
		}
		/* create a new buffer for the HDA Disk */
		HDAStatus.state							= DSKSTATE_POFF;
		HDAStatus.goalState					= DSKSTATE_POFF;
		HDAStatus.MAXCYLINDER				= HDA_MAXCYLINDER;
		HDAStatus.MAXSECTOR					= HDA_MAXSECTOR;
		HDAStatus.SECTORSIZE				= HDA_SECTORSIZE;
		HDAStatus.PON_DELAY					= HDA_PON_DELAY;
		HDAStatus.POFF_DELAY				= HDA_POFF_DELAY;
		HDAStatus.IRQ								= HDA_IRQ;
		HDAStatus.CMDREG						= HDA_CMDREG;
		HDAStatus.DATAREGS					= HDA_DATAREGS;
		HDAStatus.STPC							= HDA_STPC;
		HDAStatus.STPS							= HDA_STPS;
		HDAStatus.nextCylinderEvent = HDA_STPC;
		HDAStatus.nextSectorEvent		= HDA_STPS;
		HDAStatus.delay							= 0;
		HDAStatus.targetCylinder		= 0;
		HDAStatus.targetSector			= 0;
		HDAStatus.cmdReg						= 0;
		memset((void *)HDAStatus.dataRegs,0,16*sizeof(unsigned char));
		HDAStatus.currentCylinder		= 0;
		HDAStatus.currentSector			= 0;
		/* create a new buffer for the HDA Disk */
		if((HDAStatus.buffer = (unsigned char *)malloc(HDA_SECTORSIZE))==NULL)
				trace(DEBUG_WARNING,"failed to allocate HDA Buffer \"%s\" \n",HDA_FILENAME);
	}

	if (ENABLE_HDB)
	{
		if ( (HDBStatus.dskFile = fopen(HDB_FILENAME,"r+b")) == NULL ) /* read and write mode */
		{
			/* Error while opening the file. File does not exists...
				Create a new one and init it with random values. */
			trace(DEBUG_SETUP,"Trying to build a new HDB support...\n");
			/* Create it */
			if( (HDBStatus.dskFile = fopen(HDB_FILENAME,"wb")) == NULL)
			{
				trace(DEBUG_WARNING,"failed to create HDB File named \"%s\" \n",HDB_FILENAME);
				return 0;
			}
			/* build the file header */
			fwrite(&HDB_MAXCYLINDER,sizeof(int),1,HDBStatus.dskFile);
			fwrite(&HDB_MAXSECTOR  ,sizeof(int),1,HDBStatus.dskFile);
			fwrite(&HDB_SECTORSIZE ,sizeof(int),1,HDBStatus.dskFile);
			/* init random seed */
			srand(time(NULL));
			/* for each sector */
			for( sector=0;sector<HDB_MAXCYLINDER*HDB_MAXSECTOR;sector++ )
			{
				/* we write it char by char : for ENDIANESS =) */
				for(i=0;i<HDB_SECTORSIZE;i++)
				{
					fputc( rand(), HDBStatus.dskFile);
				}
			}
			fflush(HDBStatus.dskFile);
			fclose(HDBStatus.dskFile);
			if ( (HDBStatus.dskFile = fopen(HDB_FILENAME,"r+b")) == NULL ) /* read and write mode */
			{
				trace(DEBUG_WARNING,"failed to create HDB File named %s \n",HDB_FILENAME);
				return 0;
			}
			trace(DEBUG_SETUP,"HDB creation completed\n");
		}
		else
		{
			int maxsec;
			int maxcyl;
			int sectorSize;
			/* read current HDA MAXCYLINDER */
			fread(&maxcyl,sizeof(int),1,HDBStatus.dskFile);
			fread(&maxsec,sizeof(int),1,HDBStatus.dskFile);
			fread(&sectorSize,sizeof(int),1,HDBStatus.dskFile);
			if( (maxcyl != HDB_MAXCYLINDER) ||
				  (maxsec != HDB_MAXSECTOR) ||
					(sectorSize != HDB_SECTORSIZE) )
			{
				fclose(HDBStatus.dskFile);
				trace(DEBUG_WARNING,"the current \"%s\" file used a wrong geometry(MAXCYLINDER=%d,MAXSECTOR=%d,SECTORSIZE=%d)\n",HDB_FILENAME,maxcyl,maxsec,sectorSize);
				return 0;
			}
		}

		/* create a new buffer for the HDA Disk */
		HDBStatus.state							= DSKSTATE_POFF;
		HDBStatus.MAXCYLINDER				= HDB_MAXCYLINDER;
		HDBStatus.MAXSECTOR					= HDB_MAXSECTOR;
		HDBStatus.SECTORSIZE				= HDB_SECTORSIZE;
		HDBStatus.PON_DELAY					= HDB_PON_DELAY;
		HDBStatus.POFF_DELAY				= HDB_POFF_DELAY;
		HDBStatus.IRQ								= HDB_IRQ;
		HDBStatus.CMDREG						= HDB_CMDREG;
		HDBStatus.DATAREGS					= HDB_DATAREGS;
		HDBStatus.STPC							= HDB_STPC;
		HDBStatus.STPS							= HDB_STPS;
		HDBStatus.nextCylinderEvent	= HDB_STPC;
		HDBStatus.nextSectorEvent		= HDB_STPS;
		HDBStatus.delay							= 0;
		HDBStatus.targetCylinder		= 0;
		HDBStatus.targetSector			= 0;
		HDBStatus.cmdReg						= 0;
		memset((void *)HDAStatus.dataRegs,0,16*sizeof(unsigned char));
		HDBStatus.currentCylinder		= 0;
		HDBStatus.currentSector			= 0;
		/* create a new buffer for the HDA Disk */
		if((HDBStatus.buffer = (unsigned char *)malloc(HDB_SECTORSIZE))==NULL)
				trace(DEBUG_WARNING,"failed to allocate HDB Buffer \"%s\" \n",HDA_FILENAME);
	}

	if(ENABLE_HDA)
	{
		if(!addTickHandler(1,HDA_STPS,HDAHandler))
		{
			trace(DEBUG_SETUP,"Disk handler install failed! \n");
			return 0;
		}
	}

	if(ENABLE_HDB)
	{
		if(!addTickHandler(1,HDB_STPS,HDBHandler))
		{
			trace(DEBUG_SETUP,"Disk handler install failed! \n");
			return 0;
		}
	}
	trace(DEBUG_SETUP,"Disk initialization completed.\n");
	return 1;
}

/**
 * Tools Fcts
 */
void manageDskCmd(int cmd, struct dskStatus *dsk)
{
	int i;

	if(!dsk->enable)
		return ;

	switch(cmd)
	{
	case DSK_SEEK:
		dsk->targetCylinder = 
			((dsk->dataRegs[0] & 0xFF) <<8 ) +
			( dsk->dataRegs[1] & 0xFF);
		dsk->targetSector = 
			((dsk->dataRegs[2] & 0xFF) <<8 ) +
			( dsk->dataRegs[3] & 0xFF);
		dsk->goalState = DSKSTATE_SEEK;
		break;
	case DSK_READ:
		dsk->nbop = (dsk->dataRegs[0]<<8)+dsk->dataRegs[1];
		dsk->goalState = DSKSTATE_LOAD;
		break;
	case DSK_WRITE:
		dsk->nbop = (dsk->dataRegs[0]<<8)+dsk->dataRegs[1];
		dsk->goalState = DSKSTATE_STOR;
		break;
	case DSK_FORMAT:
		for(i=0;i<dsk->SECTORSIZE;i+=4)
		{
			dsk->buffer[i] = dsk->dataRegs[2];
			dsk->buffer[i+1] = dsk->dataRegs[3];
			dsk->buffer[i+2] = dsk->dataRegs[4];
			dsk->buffer[i+3] = dsk->dataRegs[5];
		}
		dsk->nbop = (dsk->dataRegs[0]<<8)+dsk->dataRegs[1];
		dsk->goalState = DSKSTATE_FRMT;
		break;
	case DSK_GETSTATUS:
		dsk->dataRegs[0] = dsk->state ;
		dsk->dataRegs[1] = (dsk->currentCylinder>>8)&0xFF;
		dsk->dataRegs[2] = (dsk->currentCylinder)&0xFF   ;
		dsk->dataRegs[3] = (dsk->currentSector>>8)&0xFF  ;
		dsk->dataRegs[4] = (dsk->currentSector)&0xFF     ;
		break;
	case DSK_SETSTATUS:
		dsk->state = dsk->dataRegs[0] ;
		break;
	case DSK_DSKNFO:
		dsk->dataRegs[0] = (dsk->MAXCYLINDER>>8)&0xFF;
		dsk->dataRegs[1] = (dsk->MAXCYLINDER)&0xFF   ;
		dsk->dataRegs[2] = (dsk->MAXSECTOR>>8)&0xFF  ;
		dsk->dataRegs[3] = (dsk->MAXSECTOR)&0xFF     ;
		dsk->dataRegs[4] = (dsk->SECTORSIZE>>8)&0xFF ;
		dsk->dataRegs[5] = (dsk->SECTORSIZE)&0xFF    ;
		break;
	case DSK_MANUF:
		memcpy("DiskSim (c)DDGG\0",dsk->dataRegs,16);
		break;
	case DSK_DIAG:
		break;
	}
}

/**
 * Gestion des entrées/sorties associées à ce matériel.
 */
int  in_HDA_DATAREGS (int port)
{
	return HDAStatus.dataRegs[port - HDA_DATAREGS] ;
}

int  in_HDA_CMDREG   (int port)
{
	return HDAStatus.cmdReg ;
}

int  in_HDB_DATAREGS (int port)
{
	return HDBStatus.dataRegs[port - HDB_DATAREGS] ;
}

int  in_HDB_CMDREG   (int port)
{
	return HDBStatus.cmdReg ;
}

void out_HDA_DATAREGS(int port, int value)
{
	HDAStatus.dataRegs[port - HDA_DATAREGS] = value ;
}

void out_HDA_CMDREG  (int port, int value)
{
	HDAStatus.cmdReg = value;
	manageDskCmd(value,&HDAStatus); 
}

void out_HDB_DATAREGS(int port, int value)
{
	HDBStatus.dataRegs[port - HDB_DATAREGS] = value ;
}

void out_HDB_CMDREG  (int port, int value)
{
	HDAStatus.cmdReg = value;
	manageDskCmd(value,&HDBStatus); 
}

/**
 * Gestion de l'automate de disque
 */
void diskEvent(struct dskStatus *dsk)
{
	int offset;

	if(dsk->enable == 0)
		return ;
	/* Disk OFF ? */
	if(dsk->goalState == DSKSTATE_POFF)
		return ;
	
	/* Disk is getting up ? */
	if(dsk->state == DSKSTATE_POFF) /* && dsk->goalstate != OFF */
	{
		dsk->delay++;
		if(dsk->delay > dsk->PON_DELAY-1)
		{
			dsk->delay = 0 ;
			dsk->currentCylinder = 0 ;
			dsk->currentSector = 0 ;			
			dsk->state = dsk->goalState ;
		}
		return ;
	}
	if(dsk->state != dsk->goalState )
	{
		dsk->state = dsk->goalState ;
	}

	/* managment of the sector shift */
	dsk->nextSectorEvent--;
	if(dsk->nextSectorEvent == 1) /* end of the current sector pos ? */
	{
		if( (dsk->state == DSKSTATE_LOAD) ||
				(dsk->state == DSKSTATE_STOR) ||
				(dsk->state == DSKSTATE_FRMT) )
		{
			offset = 3*sizeof(int) +
								dsk->SECTORSIZE *
								(dsk->currentSector + dsk->currentCylinder * dsk->MAXSECTOR) ;
			switch(dsk->state)
			{
			case DSKSTATE_LOAD:
				fseek(dsk->dskFile,offset,SEEK_SET);
				fread(dsk->buffer,dsk->SECTORSIZE,1,dsk->dskFile);
				break;
			case DSKSTATE_STOR:
				fseek(dsk->dskFile,offset,SEEK_SET);
				fwrite(dsk->buffer,dsk->SECTORSIZE,1,dsk->dskFile);
				fflush(dsk->dskFile);
				break;
			case DSKSTATE_FRMT:
				fseek(dsk->dskFile,offset,SEEK_SET);
				fwrite(dsk->buffer,dsk->SECTORSIZE,1,dsk->dskFile);
				fflush(dsk->dskFile);
				break;
			}
			processUserIRQ(dsk->IRQ);
			dsk->nbop--;
			if( dsk->nbop <= 0 )
			{
				dsk->state			= DSKSTATE_IDLE ;
				dsk->goalState	= DSKSTATE_IDLE ;
			}
		}
	}
	if(dsk->nextSectorEvent <= 0) /* step to the next sector ? */
	{
		dsk->nextSectorEvent = dsk->STPS ;
/**/	trace(DEBUG_WARNING,"S"); /**/
		dsk->currentSector++;
		if(dsk->currentSector >= dsk->MAXSECTOR )
		{
/**/	trace(DEBUG_WARNING,"\n"); /**/
			dsk->currentSector = 0;
		}
	}

	/* management of the cylinder shift */
	if(dsk->currentCylinder != dsk->targetCylinder )
	{
		dsk->nextCylinderEvent--;
		if(dsk->nextCylinderEvent <= 0)
		{
			dsk->nextCylinderEvent = dsk->STPC ;
/**/	trace(DEBUG_WARNING,"P"); /**/
			dsk->currentCylinder += (dsk->currentCylinder<dsk->targetCylinder)?1:-1;
			if(dsk->currentCylinder >= dsk->MAXCYLINDER )
			{
				trace(DEBUG_WARNING,"Klong!!!\n");
				dsk->currentCylinder = dsk->MAXCYLINDER -1 ;
			}
		}	
	}

	/* SEEK Management */
	if(dsk->state == DSKSTATE_SEEK)
	{
		if( (dsk->currentCylinder == dsk->targetCylinder) &&
				(dsk->currentSector == dsk->targetSector) )
		{
			dsk->state = DSKSTATE_IDLE ;
			dsk->goalState = DSKSTATE_IDLE ;
			dsk->delay = 0 ;
			dsk->nextSectorEvent = dsk->STPS ;
			processUserIRQ(dsk->IRQ);
		}
	}
	return ;
}

/**
 * Gestion des Handlers de signaux d'horologe
 */

void HDAHandler()
{
	diskEvent(&HDAStatus);
}

void HDBHandler()
{
	diskEvent(&HDBStatus);
}
