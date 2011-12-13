/* 
   Create and intialize a drive using the hardware simulator.

   A minimal example of a program using the ATA interface.
   Given as such.
   
*/

#include <stdlib.h>
#include <stdio.h>

#include "hardware.h"
#include "hw_config.h"

static void
empty_it(void)
{
    return;
}

int
main(int argc, char **argv)
{
    unsigned int i;
    
    /* init hardware */
    if (init_hardware(HARDWARE_INI) == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
	exit(EXIT_FAILURE);
    }
    
    /* interrupt handlers */
    for (i=0; i<16; i++)
	IRQVECTOR[i] = empty_it;

    /* allows all IT */
    _mask(1);

    /* empty main code */
    ; 

    /* and exit! */
    exit(EXIT_SUCCESS);
}
