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

int lancerActionneur (int argc, const char * argv[]){
//TODO: not implemented	
return 0;
}


int main (int argc, const char * argv[])
{
    printf("Bonjour\n");
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    //idValue temp = idValueWithServerMessage();
    idValue temp = idValueWithServerMessage();
    convertToFrame(temp, buffer);     
    SOCKET sock = socketConnexion();
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        printf("Erreur d'envoi de la trame d'actionneur");
    }    
    free(buffer);
    return 0;
    
}
    

