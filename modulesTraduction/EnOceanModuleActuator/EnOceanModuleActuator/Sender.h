//
//  Sender.h
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#ifndef EnOceanModuleActuator_Sender_h
#define EnOceanModuleActuator_Sender_h

typedef unsigned int socklen_t;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Listener.h"


#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define PORT 5000
#define ADRESSE "127.0.0.1"
#define FRAME_SIZE_ENOCEAN 28

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

/**
 * Crée une socket sur l'adresse ADRESSE et le port PORT défini dans Listener.h
 */
SOCKET socketConnexion(void);

void pipeReceiveSocketSend (FILE* pipe, SOCKET sock);






#endif
