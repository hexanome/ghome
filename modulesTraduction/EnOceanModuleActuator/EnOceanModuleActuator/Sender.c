//
//  Sender.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"

#define CTOX(c) (c - '0')


char stringToBinary (char str[]) {
    return CTOX(str[1]) + 16 * CTOX(str[0]);
}

tcpFrameType tcpFrameCreationWith(idValue value){
    tcpFrameType frame;
    frame.H_SEQLENGTH = stringToBinary("3B");
    
    
    
}