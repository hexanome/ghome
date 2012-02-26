/*
 ============================================================================
 Name        : TestModuleTraduction.c
 Description : Test unitaire du Module Traduction, afin de vérifier la bonne
 interprétation des trames reçues.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "TestModuleTraduction.h"

/**
 * Permet de comparer 2 structures de données de type Frame
 * frame0 - 1ère structure à comparer
 * frame1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int tcpFrameComparator(tcpFrameType frame0, tcpFrameType frame1) {

	if ( frame0.H_SEQLENGTH == frame1.H_SEQLENGTH
			&& frame0.ORG==frame1.ORG
			&& frame0.DATA_BYTE0==frame1.DATA_BYTE0
			&& frame0.DATA_BYTE1==frame1.DATA_BYTE1
			&& frame0.DATA_BYTE2==frame1.DATA_BYTE2
			&& frame0.DATA_BYTE3==frame1.DATA_BYTE3
			&& strcmp(frame0.ID_BYTE0,frame1.ID_BYTE0)==0
			&& strcmp(frame0.ID_BYTE1,frame1.ID_BYTE1)==0
			&& strcmp(frame0.ID_BYTE2,frame1.ID_BYTE2)==0
			&& strcmp(frame0.ID_BYTE3,frame1.ID_BYTE3)==0
			&& frame0.STATUS==frame1.STATUS
			&& frame0.CHECKSUM== frame1.CHECKSUM)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Permet de comparer 2 structures de données de type enOceanMessage
 * message0 - 1ère structure à comparer
 * message1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int enOceanMessageComparator(enOceanMessage message0, enOceanMessage message1) {

	if ( strcmp(message0.sensorID,message1.sensorID)==0
			&& message0.value0==message1.value0
			&& message0.value1==message1.value1
			&& message0.value2==message1.value2
			&& message0.value3==message1.value3)
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
		printf("###########################TEST n° %d #############################\n",i);
		/*Variable pour surveiller l'état du test */
		int testState = FALSE;

		/*Test de la création de la structure de données à partir de la trame */
		tcpFrameType test1 = tcpFrameCreation(testFrameArray.frames[i]);

		if ( tcpFrameComparator(testTcpFrameArray.tcpFrames[i],test1)!=0 )
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
			/*Test de la création du message à partir de la structure de données*/
			enOceanMessage test2 = enOceanMessageCreation(test1);
			if ( enOceanMessageComparator(testEnOceanMessageArray.messages[i],test2)!=0)
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
		printf("#######################FIN TEST n° %d #############################\n",i);
	}
	printf("#####################TOTAL REUSSI %d/4 ############################\n",totalSucces);
	return EXIT_SUCCESS;
}

