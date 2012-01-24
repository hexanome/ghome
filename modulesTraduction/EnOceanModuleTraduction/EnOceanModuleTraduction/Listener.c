//
//  Listener.c
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 10/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Listener.h"



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

int socketFrameReception(SOCKET aSocket, char* buffer)
{
    if (aSocket != SOCKET_ERROR ) {

        long n = 0;        
        if((n = recv(aSocket, buffer, FRAME_SIZE_ENOCEAN, 0)) < 0)
        {
            printf("%s\n",buffer);
            printf("erreur %ld \n", n);
            return -1;
        }
        buffer[n] = '\0';
        printf("%s \n",buffer);
    }
    return 0;
}

/**
 * Convertit un couple de caractères hexadécimaux en binaire
 */
char hexToBinary (char* hexValue)
{
    return hexValue[0] - '0' + 16*(hexValue[1]-'0');
}

/**
 * Créer la structure de données de la trame à partir de la chaine de caractères récupérés
 */
struct tcpFrame tcpFrameCreation (char* buffer) 
{
    struct tcpFrame aFrame;
    buffer+=4;
    aFrame.H_SEQLENGTH = hexToBinary(buffer);buffer+=6;
    aFrame.ORG = hexToBinary(buffer);buffer+=8;
    aFrame.DATA_BYTE3 = hexToBinary(buffer);buffer+=8;
    aFrame.DATA_BYTE2 = hexToBinary(buffer);buffer+=8;
    aFrame.DATA_BYTE1 = hexToBinary(buffer);buffer+=8;
    aFrame.DATA_BYTE0 = hexToBinary(buffer);buffer+=8;
    aFrame.ID_BYTE3 = hexToBinary(buffer);buffer+=8;
    aFrame.ID_BYTE2 = hexToBinary(buffer);buffer+=8;
    aFrame.ID_BYTE1 = hexToBinary(buffer);buffer+=8;
    aFrame.ID_BYTE0 = hexToBinary(buffer);buffer+=8;
    aFrame.STATUS = hexToBinary(buffer);buffer+=8;
    aFrame.CHECKSUM = hexToBinary(buffer);
    return aFrame;
}










