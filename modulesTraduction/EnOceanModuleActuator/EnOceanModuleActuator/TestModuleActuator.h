#ifndef TestModuleActuator_h
#define TestModuleActuator_h

#include "Listener.h"
#include "Sender.h"

#define TRUE 1
#define FALSE 0

/**
 * Les messages de tests provenant du serveur
 */
struct structMessageArray{
	char messages[4][28];
};

struct structMessageArray testMessageArray =
{
		.messages[0] = "DO FF9F1E03 1",
		.messages[1] = "DO 0004E957 0",
		.messages[2] = "DO 4123FF82 0",
		.messages[3] = "DO 336667AA 1",
};

/**
 * Les structures de données de référence
 */
struct structTestIdValueArray{
	idValue idValues[4];
};

struct structTestIdValueArray testIdValueArray =
{
		.idValues[0]= {"FF9F1E03",1},
		.idValues[1]= {"0004E957",0},
		.idValues[2]= {"4123FF82",0},
		.idValues[3]= {"336667AA",1}
};

/**
 * Les trames de références
 */
struct structFrameArray{
	char frames[4][29];
};

struct structFrameArray testFrameArray =
{
		.frames[0] = "A55A6B0550000000FF9F1E033000\0",
		.frames[1] = "A55A6B05570000000004E9573000\0",
		.frames[2] = "A55A6B05570000004123FF823000\0",
		.frames[3] = "A55A6B0550000000336667AA3000\0",
};

/**
 * Permet de comparer 2 structures de données de type idValue
 * idValue0 - 1ère structure à comparer
 * idValue1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int idValueComparator(idValue idValue0, idValue idValue1);

#endif
