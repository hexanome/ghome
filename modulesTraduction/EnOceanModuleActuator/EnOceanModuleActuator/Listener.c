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
}

idValue idValueWithServerMessage(){
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    buffer = "DO FF9F1E03 1";
    //while (fgets(buffer,BUFFER_RECEIVE_SIZE,stdin)) {
    idValue value = parseBuffer(buffer);
        /*int i = 0;
        for (i = 0;i < 8 ; i++) {
            value.ID[i] = buffer[i+3];
        }
        value.value = buffer[12];*/
        
        return value;
    free(buffer);
    //}
    
    
}

