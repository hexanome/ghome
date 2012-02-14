//
//  Listener.h
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 07/02/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#ifndef EnOceanModuleActuator_Listener_h
#define EnOceanModuleActuator_Listener_h
#define BUFFER_RECEIVE_SIZE 30

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct idValue {
    char ID[10];
    int value;
}idValue;

//Frame Creation
idValue parseBuffer(char* buffer);
idValue idValueWithServerMessage();

#endif
