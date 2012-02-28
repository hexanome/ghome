//
//  main.c
//  EnOceanModuleActuator
//
//

#include <stdio.h>
#include "Sender.h"
#include "Listener.h"

void lancerActionneur (void* args)
{
 /*   printf("Bonjour\n");
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    //idValue temp = idValueWithServerMessage();
    idValue temp = idValueWithServerMessage();
    convertToFrame(temp, buffer);     
        if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        printf("Erreur d'envoi de la trame d'actionneur");
    }    
    free(buffer);
    return 0;*/
    FILE *pipe;    
    // open a named pipe
    pipe = fopen("./to-actuator", "r");
    SOCKET sock = socketConnexionActuator();
    pipeReceiveSocketSend(pipe, sock);
    printf("test");
    fclose(pipe);
printf("test");
    close(sock);
printf("test");    
}
