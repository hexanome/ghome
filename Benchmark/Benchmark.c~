#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gthread.h"
#include "gmem.h"

#define VALEUR_BOUCLE 1000000
#define VALEUR_BOUCLE_THREAD 50

void fonctionInfinie(void* args);

void main()
{
	clock_t j,k,l,m;
	void* mem[VALEUR_BOUCLE];	
	int e =0;
	init();
	j = clock();
	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		mem[e] = gmalloc(16);
	}
	k = clock();
	printf("Temps pour allouer %d fois 16 octets : %f \n",VALEUR_BOUCLE,((double)k-(double)j)/CLOCKS_PER_SEC);
	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		gfree(mem[e]);
	}
	l = clock();
	printf("Temps pour liberer %d fois 16 octets : %f\n",VALEUR_BOUCLE,((double)l-(double)k)/CLOCKS_PER_SEC);
	for(e=0;e<VALEUR_BOUCLE_THREAD;e++)	
	{
		create_ctx(16384, fonctionInfinie, NULL);
	}
	m = clock();
	printf("Temps pour creer %d contexte : %f\n",VALEUR_BOUCLE_THREAD,((double)m-(double)l)/CLOCKS_PER_SEC);
}

void fonctionInfinie(void* args)
{
	int i=0;
	while(1)
	{
		i++;
		if(i>160000)
			i=0;
	}
}
