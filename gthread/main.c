#include <stdio.h>
#include <stdlib.h>
#include "gthread.h"
#include "gmem.h"

void f_ping(void *args)
{
	while(1) {
		printf("A") ;
		printf("B") ;
		printf("C") ;
	}
}
void f_pong(void *args)
{
	while(1) {
		printf("1") ;
		printf("2") ;
	}
}
void f_pang(void *args)
{
	while(1) {
		printf("*") ;
		printf("-") ;
	}
}

int main(int argc, char *argv[])
{
	int retour=0;	
	init();
	/*Spawn du module Traduction*/
	retour+=create_ctx(16384, f_ping, NULL);
	/*Spawn du module Actionneur*/
	retour+=create_ctx(16384, f_pong, NULL);
	if(retour==0)
	{
		start_sched();
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}



