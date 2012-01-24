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

enum sensorType {
    enOceanRockerSwitch = 1,
    enOceanTemperatureSensor = 2,
    enOceanSingleContact = 3,
    enOceanLightSensor = 4,
};

typedef struct enOceanMessage
{
    enum sensorType type;
    char sensorID[5];
    float value1;
    float value2;
    float value3;    
}enOceanMessage;

struct enOceanMessage enOceanMessageCreation(struct tcpFrame aFrame);
int sendSensorStateByPipe (struct enOceanMessage aMessage);


#endif
