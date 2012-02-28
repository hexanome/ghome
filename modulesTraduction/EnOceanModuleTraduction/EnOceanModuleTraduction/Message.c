//
//  Message.c
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 17/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Message.h"

/**
 * Créer un message contenant l'ID du capteur et les 4 valeurs associés à partir d'une trame
 * aFrame - la trame à transformer en messsage
 * return - le message créer
 */
enOceanMessage enOceanMessageCreation(tcpFrameType aFrame)
{
    struct enOceanMessage anOceanMessage; 

    anOceanMessage.value0 = aFrame.DATA_BYTE0;
    anOceanMessage.value1 = aFrame.DATA_BYTE1;
    anOceanMessage.value2 = aFrame.DATA_BYTE2;
    anOceanMessage.value3 = aFrame.DATA_BYTE3;
    //Conversion des 4 char[2] d'ID en une seule chaine de caractères de 8
    anOceanMessage.sensorID[8] = '\0';
    anOceanMessage.sensorID[7] = aFrame.ID_BYTE0[1];
    anOceanMessage.sensorID[6] = aFrame.ID_BYTE0[0];
    anOceanMessage.sensorID[5] = aFrame.ID_BYTE1[1];
    anOceanMessage.sensorID[4] = aFrame.ID_BYTE1[0];
    anOceanMessage.sensorID[3] = aFrame.ID_BYTE2[1];
    anOceanMessage.sensorID[2] = aFrame.ID_BYTE2[0];
    anOceanMessage.sensorID[1] = aFrame.ID_BYTE3[1];
    anOceanMessage.sensorID[0] = aFrame.ID_BYTE3[0];

    return anOceanMessage;
}


/**
 * Envoie un message grâce à printf en utilisant une syntaxe définie dans un protocole
 * aMessage - le message à envoyer
 */
int sendSensorStateByPipe (enOceanMessage aMessage, FILE* pipe) 
{
    //char* buffer = malloc(MAX_SIZE);
    //sprintf(buffer,"GIVE_SENSORSTATE ,%s ,%d,%d ,%d ,%d \nSTOP\n", aMessage.sensorID,  aMessage.value0,aMessage.value1,aMessage.value2,aMessage.value3);    
    //printf("%s",buffer);
printf("Test1\n");
printf("GIVE_SENSORSTATE %s %d %d %d %d \nSTOP\n", aMessage.sensorID,  aMessage.value0,aMessage.value1,aMessage.value2, aMessage.value3);
    fprintf(pipe, "GIVE_SENSORSTATE %s %d %d %d %d\nSTOP\n", aMessage.sensorID,  aMessage.value0,aMessage.value1,aMessage.value2, aMessage.value3);
fflush(pipe);
printf("Test2\n");
    //unsigned long \\ = fwrite(buffer, sizeof(char),strlen(buffer),pipe);
    //printf("%ld",recu);
    //free(buffer);
    
    return 0;    
}

