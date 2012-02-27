//
//  Sender.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"

/**
 * Crée une socket sur l'adresse ADRESSE et le port PORT défini dans Sender.h
 */
SOCKET socketConnexion(void)
{
    
    int erreur = 0;
    
    SOCKET sock;
    SOCKADDR_IN sin;
    
    if(!erreur)
    {
        /* Création de la socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        /* Configuration de la connexion */
        sin.sin_addr.s_addr = inet_addr(ADRESSE);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);
        
        /* Si le client arrive à se connecter */
        if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
            printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        else
            printf("Impossible de se connecter\n");
    }
    
    return sock;
}


void pipeReceiveSocketSend (FILE* pipe, SOCKET sock) {
    char* bufferPipe = malloc(BUFFER_RECEIVE_SIZE);
    char* bufferFrame = malloc(BUFFER_RECEIVE_SIZE);
    
    //buffer = "DO FF9F1E03 1";
    idValue envoi;
    
    while (getline(&bufferPipe, BUFFER_RECEIVE_SIZE, pipe)) {
        
        envoi = parseBuffer(bufferPipe); ;
        convertToFrame(envoi, bufferFrame);
        if(send(sock, bufferFrame, strlen(bufferFrame), 0) < 0)
        {
            printf("Erreur d'envoi de la trame d'actionneur");
        }  
        
        
    }
    free(bufferPipe);
    free(bufferFrame);
    return;
    
}







