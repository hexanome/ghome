//
//  Message.h
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 17/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#ifndef EnOceanModuleTraduction_Message_h
#define EnOceanModuleTraduction_Message_h

#include "Listener.h"
#include <string.h>

/**
 * Le message à envoyer au serveur, contenant l'ID du capteur et les 4 valeurs associées
 */
typedef struct enOceanMessage
{
    char sensorID[9];
    int value0;
    int value1;
    int value2;
    int value3;
}enOceanMessage;

/**
 * Crée un message contenant l'ID du capteur et les 4 valeurs associés à partir d'une trame
 * aFrame - la trame à transformer en messsage
 * return - le message créer
 */
enOceanMessage enOceanMessageCreation (tcpFrameType aFrame);

/**
 * Envoie un message grâce à printf en utilisant une syntaxe définie dans un protocole
 * aMessage - le message à envoyer
 */
int sendSensorStateByPipe (enOceanMessage aMessage);


#endif
