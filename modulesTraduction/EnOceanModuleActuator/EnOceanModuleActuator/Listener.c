//
//  Listener.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 07/02/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Listener.h"


/**
 * Parse la chaîne de caractères reçue, qui est de la forme "DO 12345678 1"
 * buffer - la chaîne de caractères à parser
 * return - la structure de données idValue qui contient l'ID de l'actionneur et la valeur qu'il doit prendre
 */
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

/**
 *Récupère le message du serveur et le parse
 *return - la structure de donnée contenant l'ID et la valeur de l'actionneur
 */
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

