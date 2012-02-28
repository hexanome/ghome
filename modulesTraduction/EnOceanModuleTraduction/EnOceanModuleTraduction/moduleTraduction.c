/*
 Redirection :
 $ ssh -L 5000:134.214.105.28:5000 alefoulon@iftpserv2.insa-lyon.fr 
 Ecoute:
 $ telnet localhost 5000
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "Listener.h"
#include "Message.h"
#include "string.h"

void lancerTraducteur (void* args)
{
	char* buffer = malloc(FRAME_SIZE_ENOCEAN);
    FILE *pipe;  
    // open a named pipe
	
    pipe = fopen("./from-sensor", "a");

	if (pipe == NULL){printf("ERROR\n");}

	SOCKET sock = socketConnexion();
    while (sock != SOCKET_ERROR)
    {
	socketFrameReception(sock,buffer);
	printf("%s\n",buffer);
	tcpFrameType myFrame = tcpFrameCreation(buffer);
	enOceanMessage myMessage = enOceanMessageCreation(myFrame);
	sendSensorStateByPipe(myMessage, pipe);
    }
    free(buffer);
}

