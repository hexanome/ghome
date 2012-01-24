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