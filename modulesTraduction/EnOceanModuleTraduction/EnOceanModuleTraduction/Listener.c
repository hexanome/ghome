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
	int Decimal;
	int Unite;

	printf("%c%c\n",*hexValue,*(hexValue+1));
	//Il n'y pas continuité dans la table ASCII entre 9 et A
	if ( *(hexValue) > '9')
	{
		printf("decimal:%ld\n",*hexValue -'A'+10);
		Decimal = *hexValue -'A'+10;
	}
	else
	{
		printf("decimal:%ld\n",*hexValue -'0');
		Decimal = *hexValue -'0';
	}
	if ( *(hexValue+1) > '9' )
	{
		printf("unite:%ld\n",*(hexValue+1)-'A'+10);
		Unite = *(hexValue+1) -'A'+10;
	}
	else
	{
		printf("unite:%ld\n",*(hexValue+1)-'0');
		Unite = *(hexValue+1) -'0';
	}
	printf("%ld\n",Unite+16*(Decimal));
	return Unite+16*(Decimal);
}

/**
 * Créer la structure de données de la trame à partir de la chaine de caractères récupérés
 */
struct tcpFrame tcpFrameCreation (char* buffer) 
{
    struct tcpFrame aFrame;
    buffer+=4;
    aFrame.H_SEQLENGTH = hexToBinary(buffer);buffer+=2;
    aFrame.ORG = hexToBinary(buffer);buffer+=2;
    aFrame.DATA_BYTE3 = hexToBinary(buffer);buffer+=2;
    aFrame.DATA_BYTE2 = hexToBinary(buffer);buffer+=2;
    aFrame.DATA_BYTE1 = hexToBinary(buffer);buffer+=2;
    aFrame.DATA_BYTE0 = hexToBinary(buffer);buffer+=2;
    aFrame.ID_BYTE3 = hexToBinary(buffer);buffer+=2;
    aFrame.ID_BYTE2 = hexToBinary(buffer);buffer+=2;
    aFrame.ID_BYTE1 = hexToBinary(buffer);buffer+=2;
    aFrame.ID_BYTE0 = hexToBinary(buffer);buffer+=2;
    aFrame.STATUS = hexToBinary(buffer);buffer+=2;
    aFrame.CHECKSUM = hexToBinary(buffer);
    return aFrame;
}










