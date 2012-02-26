//
//  Sender.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"
//#include "EnOceanModuleActuator/EnOceanModuleActuator/Sender.h"

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


/**
 * Permet de créer une trame à partir de l'ID et la valeur de l'actionneur
 * idValue - la structure de données contenant l'ID et la valeur de l'actionneur
 * buffer - la chaîne de caractères représentant la trame ainsi créée
 */
void convertToFrame(idValue idValue, char* buffer)
{
    strcat(buffer, "A55A6B05");
    if (idValue.value == 1)
    {
        strcat(buffer, "50000000");
        
    }else if (idValue.value == 0)
    {
        strcat(buffer, "57000000");
    }
    strcat(buffer, idValue.ID);
    strcat(buffer, "3000\0");
   // printf("%s",buffer);

}






