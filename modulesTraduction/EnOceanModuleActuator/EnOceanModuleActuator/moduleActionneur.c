//
//  main.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "EnOceanModuleActuator/EnOceanModuleActuator/Sender.h"
#include "EnOceanModuleActuator/EnOceanModuleActuator/Listener.h"

void lancerActionneur (void* args){
    printf("Bonjour\n");
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    //tcpFrameWithServerMessageWithBuffer("DO ABCDE 1");
    idValue temp = parseBuffer("DO ABCDE 1");
    printf("%s\n",temp.ID);
    printf("%c\n",temp.value);
    free(buffer);
}


