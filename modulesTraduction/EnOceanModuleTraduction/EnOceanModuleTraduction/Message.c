//
//  Message.c
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 17/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Message.h"



struct enOceanMessage enOceanMessageCreation(struct tcpFrame aFrame)
{
    struct enOceanMessage anOceanMessage; 
    char * concat1,concat2;
    //Message en mou
    anOceanMessage.value0 = aFrame.DATA_BYTE0;
    anOceanMessage.value1 = aFrame.DATA_BYTE1;
    anOceanMessage.value2 = aFrame.DATA_BYTE2;
    anOceanMessage.value3 = aFrame.DATA_BYTE3;
    //Conversion des 4 char[2] d'ID en une seule chaine de caractères de 8
    anOceanMessage.sensorID[0] = aFrame.ID_BYTE0[0];
    anOceanMessage.sensorID[1] = aFrame.ID_BYTE0[1];
    anOceanMessage.sensorID[2] = aFrame.ID_BYTE1[0];
    anOceanMessage.sensorID[3] = aFrame.ID_BYTE1[1];
    anOceanMessage.sensorID[4] = aFrame.ID_BYTE2[0];
    anOceanMessage.sensorID[5] = aFrame.ID_BYTE2[1];
    anOceanMessage.sensorID[6] = aFrame.ID_BYTE3[0];
    anOceanMessage.sensorID[7] = aFrame.ID_BYTE3[1];





    //Mise en place d'un message en dur avec une id et des datas
    //anOceanMessage.value1 = 0.23;
    //strcpy(anOceanMessage.sensorID,"E123");
    return anOceanMessage;
    
}



int sendSensorStateByPipe (enOceanMessage aMessage) 
{
    int fd;
    dup2(fd, STDOUT_FILENO); /* connect the write side with stdout */
    
    printf("GIVE_SENSORSTATE  ,%s ,%f ,%f ,%f \nSTOP\n", aMessage.sensorID, aMessage.value1,aMessage.value2,aMessage.value3);
    return 0;    
}

