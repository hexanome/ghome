//
//  main.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
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
    pipe = fopen("/ghome/to-actuator", O_RDONLY);
    SOCKET sock = socketConnexionActuator();
    
    pipeReceiveSocketSend(pipe, sock);
    
    fclose(pipe);
    close(sock);    
}
