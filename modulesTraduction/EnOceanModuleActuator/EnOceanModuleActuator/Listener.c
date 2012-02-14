//
//  Listener.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 07/02/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Listener.h"


idValue idValueWithServerMessage(){
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    buffer = "DO FF9F1E03 1";
    //while (fgets(buffer,BUFFER_RECEIVE_SIZE,stdin)) {
        idValue value;
        int i = 0;
        for (i = 0;i < 8 ; i++) {
            value.ID[i] = buffer[i+3];
        }
        value.value = buffer[12];
        
        return value;
    free(buffer);
    //}
    
    
}

