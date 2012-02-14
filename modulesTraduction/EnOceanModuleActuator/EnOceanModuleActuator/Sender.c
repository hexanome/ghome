//
//  Sender.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"

#define CTOX(c) (c - '0')


char stringToBinary (char str[]) {
    return CTOX(str[1]) + 16 * CTOX(str[0]);
}

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

tcpFrameType tcpFrameCreationWith(idValue value){
    tcpFrameType frame;
    frame.H_SEQLENGTH = stringToBinary("6B");
    frame.ID_BYTE0 = stringToBinary("03");
    frame.ID_BYTE1 = stringToBinary("FF");
    frame.ID_BYTE2 = stringToBinary("9F");
    frame.ID_BYTE3 = stringToBinary("1E");
    // A remplacer par la value.
    if (value.value == '1') {
        frame.DATA_BYTE0 = stringToBinary("50");
        frame.DATA_BYTE1 = stringToBinary("00");
        frame.DATA_BYTE2 = stringToBinary("00");
        frame.DATA_BYTE3 = stringToBinary("00");
               
    }else{
        frame.DATA_BYTE0 = stringToBinary("51");
        frame.DATA_BYTE1 = stringToBinary("00");
        frame.DATA_BYTE2 = stringToBinary("00");
        frame.DATA_BYTE3 = stringToBinary("00");
    }
    frame.CHECKSUM = stringToBinary("01");    
    frame.STATUS = stringToBinary("01");
    return frame;
}

void convertToBuffer(tcpFrameType frame, char* buffer){
    
}