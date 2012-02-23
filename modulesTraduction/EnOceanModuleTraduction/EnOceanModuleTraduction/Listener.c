//
//  Listener.c
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 10/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Listener.h"


/**
 * Créer une socket sur l'adresse ADRESSE et le port PORT défini dans Listener.h
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
 * Permet la réception de trames à partir d'une socket
 * aSocket - la socket à écouter
 * buffer - le buffer pour stocker la trame
 */
int socketFrameReception(SOCKET aSocket, char* buffer)
{
    if (aSocket != SOCKET_ERROR ) {

        long n = 0;        
        if((n = recv(aSocket, buffer, FRAME_SIZE_ENOCEAN, 0)) < 0)
        {
            printf(" Erreur %s\n",buffer);
            printf("erreur %ld \n", n);
            return -1;
        }
        buffer[n] = '\0';
       // printf("%s \n",buffer);
    }
    return 0;
}

/**
 * Convertit une chaîne de 2 caractères de long représentant un encodage hexadécimal en un entier
 */
int hexToInt (char* hexValue)
{
	int Resultat;
	int Decimal;
	int Unite;

	//printf("%c%c\n",*hexValue,*(hexValue+1));
	//Il n'y pas continuité dans la table ASCII entre 9 et A
	if ( *(hexValue) > '9')
	{
		//printf("decimal:%d\n",*hexValue -'A'+10);
		Decimal = *hexValue -'A'+10;
	}
	else
	{
		//printf("decimal:%d\n",*hexValue -'0');
		Decimal = *hexValue -'0';
	}
	if ( *(hexValue+1) > '9' )
	{
		//printf("unite:%d\n",*(hexValue+1)-'A'+10);
		Unite = *(hexValue+1) -'A'+10;
	}
	else
	{
		//printf("unite:%d\n",*(hexValue+1)-'0');
		Unite = *(hexValue+1) -'0';
	}
	//Le resultat de la conversion
	Resultat = Unite+16*Decimal;
	return Resultat;
}

/**
 * Créer la structure de données de la trame à partir de la chaîne de caractères récupérés
 * La chaine de caractère se présente sous la forme d'une chaine de 28 caractères de longs,
 * chaque doublet représentant un octet codé en hexadecimal
 * buffer - la chaîne de caractère  à convertir
 */
struct tcpFrame tcpFrameCreation (char* buffer) 
{
    struct tcpFrame aFrame;
    /*Les 2 premiers octets (4 caractères) sont des octets de synchronisation*/
    buffer+=4;
    aFrame.H_SEQLENGTH = hexToInt(buffer);buffer+=2;
    aFrame.ORG = hexToInt(buffer);buffer+=2;
    aFrame.DATA_BYTE3 = hexToInt(buffer);buffer+=2;
    aFrame.DATA_BYTE2 = hexToInt(buffer);buffer+=2;
    aFrame.DATA_BYTE1 = hexToInt(buffer);buffer+=2;
    aFrame.DATA_BYTE0 = hexToInt(buffer);buffer+=2;
    aFrame.ID_BYTE3[0] = *(buffer);aFrame.ID_BYTE3[1] = *(buffer+1);buffer+=2;
    aFrame.ID_BYTE2[0] = *(buffer);aFrame.ID_BYTE2[1] = *(buffer+1);buffer+=2;
    aFrame.ID_BYTE1[0] = *(buffer);aFrame.ID_BYTE1[1] = *(buffer+1);buffer+=2;
    aFrame.ID_BYTE0[0] = *(buffer);aFrame.ID_BYTE0[1] = *(buffer+1);buffer+=2;
    aFrame.STATUS = hexToInt(buffer);buffer+=2;
    aFrame.CHECKSUM = hexToInt(buffer);
    return aFrame;
}










