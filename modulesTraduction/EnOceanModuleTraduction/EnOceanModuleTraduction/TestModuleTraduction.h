#ifndef TestModuleTraduction_h
#define TestModuleTraduction_h

#include "Listener.h"
#include "Message.h"

#define TRUE 1;
#define FALSE 0;

/**
 * Les trames de tests
 */
struct structFrameArray{
	char frames[4][28];
};

struct structFrameArray testFrameArray =
{
		.frames[0] = "A55A0B070084990F0004E9570001",
		.frames[1] = "A55A0B0712721F004123FF820001",
		.frames[2] = "A55A0B0725880A13336667AA0001",
		.frames[3] = "A55A0B070BA55A07007FACB90001",
};

/**
 * Les structures de données de référence
 */
struct structTestTcpFrameArray{
	tcpFrameType tcpFrames[4];
};

struct structTestTcpFrameArray testTcpFrameArray =
{
		.tcpFrames[0]= {11,7,0,132,153,15,"00","04","E9","57",0,1},
		.tcpFrames[1]= {11,7,18,114,31,00,"41","23","FF","82",0,1},
		.tcpFrames[2]= {11,7,37,136,10,19,"33","66","67","AA",0,1},
		.tcpFrames[3]= {11,7,11,165,90,7,"00","7F","AC","B9",0,1}
};

/*
 * Les message de référence
 */
struct structTestenOceanMessageArray {
	enOceanMessage messages[4];
};

struct structTestenOceanMessageArray testEnOceanMessageArray = {
		.messages[0] = {"0004E957",15,153,132,0},
		.messages[1] = {"4123FF82",0,31,114,18},
		.messages[2] = {"336667AA",19,10,136,37},
		.messages[3] = {"007FACB9",7,90,165,11},
};

/**
 * Permet de comparer 2 structures de données de type Frame
 * frame0 - 1ère structure à comparer
 * frame1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int tcpFrameComparator(tcpFrameType frame0, tcpFrameType frame1);

/**
 * Permet de comparer 2 structures de données de type enOceanMessage
 * message0 - 1ère structure à comparer
 * message1 - 2ème structure à comparer
 * return - TRUE si elles sont identiques, FALSE sinon
 */
int enOceanMessageComparator(enOceanMessage message0, enOceanMessage message1);

#endif
