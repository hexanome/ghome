/*
 ============================================================================
 Name        : TestModuleActuator.c
 Description : Test unitaire du Module Actuator, afin de vérifier le bon parsing
 du message reçu du serveur.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "TestModuleActuator.h"

/**
 * Permet de comparer 2 structures de données de type idValue
 * idValue0 - 1ère structure à comparer
 * idValue1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int idValueComparator(idValue idValue0, idValue idValue1) {

	if ( strcmp(idValue0.ID,idValue1.ID)==0
			&& idValue0.value==idValue1.value)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


int main(void) {

    int totalSucces = 0;
    	/*Lancement des 4 tests */
    	int i = 0;
    	for (i; i < 4; i++)
    	{
    		char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    		printf("###########################TEST n° %d #############################\n",i);
    		/*Variable pour surveiller l'état du test */
    		int testState = FALSE;

    		/*Test de la création de la structure de données à partir du message reçu depuis le serveur */
    		idValue test = parseBuffer(testMessageArray.messages[i]);

    		if ( idValueComparator(testIdValueArray.idValues[i],test)!=0 )
    		{
    			puts("Etape 1 : OK");
    			testState = TRUE;
    		}
    		else
    		{
    			puts("Etape 1 : KO");
    			testState = FALSE;
    		}

    		/* On continue si le test précédent à fonctionner*/
    		if ( testState == 1 )
    		{
    			/*Test de la création de la trame à partir de la structure de données*/
    			convertToFrame(test,buffer);
    			if ( strcmp(buffer, testFrameArray.frames[i])==0)
    			{
    				puts("Etape 2 : OK");
    				testState = TRUE;
    				totalSucces++;
    			}
    			else
    			{
    				puts("Etape 2 : KO");
    				testState = FALSE;
    			}
    		}
    		free(buffer);
    		printf("#######################FIN TEST n° %d #############################\n",i);
    	}
    	printf("#####################TOTAL REUSSI %d/4 ############################\n",totalSucces);

	return EXIT_SUCCESS;
}
