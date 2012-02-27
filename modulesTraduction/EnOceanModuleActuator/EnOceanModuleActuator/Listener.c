//
//  Listener.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 07/02/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include "Listener.h"
//#include "EnOceanModuleActuator/EnOceanModuleActuator/Listener.h"


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

    phrase[i] = '\0';
    i++;
    
    if (!strcmp(phrase, "DO")) {
        int start = i;
        for (;((buffer[i] != ' ') && (i < BUFFER_RECEIVE_SIZE)); i++) {
            phrase[i-start] = buffer[i];
        }

        phrase[i-start] = '\0';
        strcpy(retour.ID, phrase);
        /*Conversion d'un caractère en entier*/
        retour.value = buffer[i+1]-'0';
    }
    return retour;
}

/**
 * Permet de créer une trame à partir de l'ID et la valeur de l'actionneur
 * idValue - la structure de données contenant l'ID et la valeur de l'actionneur
 * buffer - la chaîne de caractères représentant la trame ainsi créée
 */
void convertToFrame(idValue idValue, char* buffer)
{
    strcat(buffer, "A55A6B05");
    if (idValue.value == 1)
    {
        strcat(buffer, "50000000");
        
    }else if (idValue.value == 0)
    {
        strcat(buffer, "57000000");
    }
    strcat(buffer, idValue.ID);
    strcat(buffer, "3000\0");
    printf("%s",buffer);
    
}









