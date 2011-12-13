/* ------------------------------
   $Id: hw_clock.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_clock.c

   Module de gestion du simulateur de TIMER materiel.
   
*/

#include <stdlib.h>
#include "hardware.h"
#include "hw_tools.h"
#include "hw_clock.h"

/**
 * Constantes specifiants les registres de configuration du materiel simule.
 * (initialisee en fonction du fichier de configuration)
 */
int    TIMER_CLOCK   = 0;
int    TIMER_PARAM   = 0;
int    TIMER_ALARM   = 0;
int    TIMER_IRQ     = 0;
int    TIMER_TICKS   = 1000;

/**
 * variables globales d'emulation des liaisons series simulees
 */
unsigned int currentClock;
unsigned int alarmSet;
unsigned int alarmDemul;
unsigned int currentDemul;
unsigned int currentAlarm;


void clockHandler();

int init_Clock()
{
  trace(DEBUG_SETUP,"trying Clock initialization...\n");
	currentClock = 0 ;
	alarmSet = 0;
  alarmDemul = 1;
	currentAlarm = 0 ;
	currentDemul = 0 ;
	if(!addTickHandler(1,TIMER_TICKS,clockHandler))
	{
		trace(DEBUG_SETUP,"Clock initialization failed! \n");
		return 0;
	}
	trace(DEBUG_SETUP,"Clock initialization completed.\n");
	return 1;
}

int in_TIMER_CLOCK(int port)
{
	return currentClock;
}

static int current_param = 0;
int in_TIMER_PARAM(int port)
{
	return current_param;
}

int in_TIMER_ALARM(int port)
{
  return currentAlarm;
}

void out_TIMER_CLOCK(int port, int value)
{
	trace(DEBUG_WARNING,"out_TIMER\n");
	currentClock = value;
}

void out_TIMER_PARAM(int port, int value)
{

	if((value & 0x80) == 0x80)
	{
		alarmSet = 0 ;
		alarmDemul = 1 ;
		currentDemul = 0 ;
		currentAlarm = 0 ;
		currentClock = 0 ;
	}

	if((value & 0x40) == 0x40)
		alarmSet = 1 ;
	else
		alarmSet = 0 ;

	if((value & 0x20) == 0x20)
	{
		switch((value&0x18)>>3)
		{
			case 0x00: 
				alarmDemul = 1 ;
				break ;
			case 0x01:
				alarmDemul = 8 ;
				break ;
			case 0x02:
				alarmDemul = 64 ;
				break ;
			case 0x03:
				alarmDemul = 512 ;
				break ;
		}		
	}

  switch(value & 0x03)
	{
		case 0x00:
			current_param = currentAlarm ;
			break;
		case 0x01:
			current_param = alarmDemul ;
			break;
		case 0x02:
			current_param = TIMER_TICKS ;
			break;
		case 0x03:
			current_param = TIMER_IRQ ;
			break;
	}

}

void out_TIMER_ALARM(int port, int value)
{
  currentAlarm = value;
	currentDemul = 0;
}

void clockHandler()
{
	currentClock++;
	if(alarmSet)
	{
		currentDemul++;
		if(currentDemul>=alarmDemul)
		{
			currentDemul = 0 ;
			currentAlarm++ ;
			if(currentAlarm == 0xFFFFFFFF)
			{
				processUserIRQ(TIMER_IRQ);
			}
		}
	}
}
