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

/**
 * La structure de données contenant l'ID de l'actionneur et sa valeur
 */
typedef struct idValue {
    char ID[10];
    int value;
}idValue;

/**
 * Parse la chaîne de caractères reçue, qui est de la forme "DO 12345678 1"
 * buffer - la chaîne de caractères à parser
 * return - la structure de données idValue qui contient l'ID de l'actionneur et la valeur qu'il doit prendre
 */
idValue parseBuffer(char* buffer);
/**
 *Récupère le message du serveur et le parse
 *return - la structure de donnée contenant l'ID et la valeur de l'actionneur
 */
idValue idValueWithServerMessage();

#endif
