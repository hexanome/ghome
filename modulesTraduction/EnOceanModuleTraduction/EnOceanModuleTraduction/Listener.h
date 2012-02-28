//
//  Listener.h
//  EnOceanModuleTraduction
//
//

#ifndef EnOceanModuleTraduction_Listener_h
#define EnOceanModuleTraduction_Listener_h


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define PORT 5000
#define ADRESSE "127.0.0.1"
#define FRAME_SIZE_ENOCEAN 28

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef unsigned int socklen_t;

typedef struct tcpFrame {
    int H_SEQLENGTH; // Frame third byte.
    int ORG;
    int DATA_BYTE3;
    int DATA_BYTE2;
    int DATA_BYTE1;
    int DATA_BYTE0;
    char ID_BYTE3[2];
    char ID_BYTE2[2];
    char ID_BYTE1[2];
    char ID_BYTE0[2];
    int STATUS;
    int CHECKSUM;
    
}tcpFrameType;

/**
 * Convertit une chaîne de 2 caractères de long représentant un encodage hexadécimal en un entier
 * hexValue - la chaîne de caractères à convertir
 */
int hexToInt (char* hexValue);

/**
 * Créer une socket sur l'adresse ADRESSE et le port PORT défini dans Listener.h
 */
SOCKET socketConnexion(void);

/**
 * Permet la réception de trames à partir d'une socket
 * aSocket - la socket à écouter
 * buffer - le buffer pour stocker la trame
 */
int socketFrameReception(SOCKET aSocket, char* buffer);


/**
 * Créer la structure de données de la trame à partir de la chaîne de caractères récupérés
 * La chaine de caractère se présente sous la forme d'une chaine de 28 caractères de longs,
 * chaque doublet représentant un octet codé en hexadecimal
 * buffer - la chaîne de caractère  à convertir
 */
tcpFrameType tcpFrameCreation (char* buffer);






#endif
