#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gthread.h"
#include "gmem.h"

#define VALEUR_BOUCLE 1000000
#define NB_OCTETS 16
#define VALEUR_BOUCLE_THREAD 50
#define TAILLE_PILE_EXECUTION 16384

void fonctionInfinie(void* args);

void main()
{
	clock_t j,k,l,m,n,o;
	void* mem[VALEUR_BOUCLE];	
	int e =0;
	init();
	j = clock();
	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		mem[e] = gmalloc(NB_OCTETS);
	}
	k = clock();
	printf("Temps pour allouer %d fois %d octets : %f \n",VALEUR_BOUCLE,NB_OCTETS,((double)k-(double)j)/CLOCKS_PER_SEC);
	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		gfree(mem[e]);
	}
	l = clock();
	printf("Temps pour liberer %d fois 16 octets : %f\n",VALEUR_BOUCLE,((double)l-(double)k)/CLOCKS_PER_SEC);
	for(e=0;e<VALEUR_BOUCLE_THREAD;e++)	
	{
		create_ctx(TAILLE_PILE_EXECUTION, fonctionInfinie, NULL);
	}
	m = clock();
	printf("Temps pour creer %d contexte contenant une pile d'execution de %d octets : %f\n",VALEUR_BOUCLE_THREAD,TAILLE_PILE_EXECUTION,((double)m-(double)l)/CLOCKS_PER_SEC);

	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		mem[e] = malloc(NB_OCTETS);
	}
	n = clock();
	printf("MALLOC Temps pour liberer %d fois 16 octets : %f\n",VALEUR_BOUCLE,((double)n-(double)m)/CLOCKS_PER_SEC);
	for(e=0;e<VALEUR_BOUCLE;e++)	
	{
		free(mem[e]);
	}
	o = clock();
	printf("FREE Temps pour allouer %d fois %d octets : %f \n",VALEUR_BOUCLE,NB_OCTETS,((double)o-(double)n)/CLOCKS_PER_SEC);

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
