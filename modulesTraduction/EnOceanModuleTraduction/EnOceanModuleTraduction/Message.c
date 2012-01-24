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
    //Mise en place d'un message en dur
    anOceanMessage.type = enOceanLightSensor;
    anOceanMessage.value1 = 0.23;
    strcpy(anOceanMessage.sensorID,"E123");
    return anOceanMessage;
    
}

int sendSensorStateByPipe (enOceanMessage aMessage) 
{
    int fd;
    dup2(fd, STDOUT_FILENO); /* connect the write side with stdout */
    
    printf("GIVE_SENSORSTATE ,%d ,%s ,%f ,%f ,%f \nSTOP\n", aMessage.type,aMessage.sensorID, aMessage.value1,aMessage.value2,aMessage.value3);
    return 0;    
}

