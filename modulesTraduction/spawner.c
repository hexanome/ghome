#include <stdio.h>
#include <stdlib.h>
#include "gthread.h"
#include "gmem.h"
#include "EnOceanModuleTraduction/EnOceanModuleTraduction/moduleTraduction.h"
#include "EnOceanModuleActuator/EnOceanModuleActuator/moduleActionneur.h"

int main(int argc, char *argv[])
{
	int retour=0;	
	init();
	/*Spawn du module Traduction*/
	retour+=create_ctx(16384, lancerTraducteur, NULL);
	/*Spawn du module Actionneur*/
	retour+=create_ctx(16384, lancerActionneur, NULL);
	if(retour==0)
	{
		start_sched();
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}



