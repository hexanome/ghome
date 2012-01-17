//
//  Message.h
//  EnOceanModuleTraduction
//
//  Created by Alexandre Lefoulon on 17/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#ifndef EnOceanModuleTraduction_Message_h
#define EnOceanModuleTraduction_Message_h

enum sensorType {
    enOceanRockerSwitch = 1,
    enOceanTemperatureSensor = 2,
    enOceanSingleContact = 3,
    enOceanLightSensor = 4,
};

struct enOceanMessage
{
    enum sensorType;
    char sensorID[4];
    float value1;
    float value2;
    float value3;
    
};

#endif
