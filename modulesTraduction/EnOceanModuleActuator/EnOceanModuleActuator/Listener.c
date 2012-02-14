//
//  Listener.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 07/02/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Listener.h"



idValue parseBuffer(char* buffer){
    int i = 0;
    char phrase[BUFFER_RECEIVE_SIZE];
    idValue retour;
    
    for (i = 0;((buffer[i] != ' ') && (i < BUFFER_RECEIVE_SIZE)); i++) {
        phrase[i] = buffer[i];
    }
    i++;
    phrase[i] = '\0';
    if (!strcmp(phrase, "DO")) {
        int start = i;
        for (;((buffer[i] != ' ') && (i < BUFFER_RECEIVE_SIZE)); i++) {
            phrase[i-start] = buffer[i];
        }
        phrase[i-start] = '\0';
        strcpy(retour.ID, phrase);
        retour.value = buffer[i+1];        
    }
    return retour;
    
}

idValue tcpFrameWithServerMessageWithBuffer(char* buffer) {
    while (fgets(buffer,BUFFER_RECEIVE_SIZE,stdin)) {
        idValue value = parseBuffer(buffer);    
        return value;
    }
}

