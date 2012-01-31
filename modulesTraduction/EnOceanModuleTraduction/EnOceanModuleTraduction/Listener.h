//
//  Listener.h
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 10/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#ifndef EnOceanModuleTraduction_Listener_h
#define EnOceanModuleTraduction_Listener_h
typedef unsigned int socklen_t;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Message.h"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define PORT 5000
#define ADRESSE "127.0.0.1"
#define FRAME_SIZE_ENOCEAN 28

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;


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
    
}tcpFrame;

//Utility Functions
int hexToInt (char* hexValue);

//Managing Socket
SOCKET socketConnexion(void);
int socketFrameReception(SOCKET aSocket, char* buffer);
int socketListen (SOCKET aSocket);

//Frame Creation
struct tcpFrame tcpFrameCreation (char* buffer);


#endif
