//
//  Listener.h
//  EnOceanModuleActuator
//
//

#ifndef EnOceanModuleActuator_Listener_h
#define EnOceanModuleActuator_Listener_h
#define BUFFER_RECEIVE_SIZE 30

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
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
 * Permet de créer une trame à partir de l'ID et la valeur de l'actionneur
 * idValue - la structure de données contenant l'ID et la valeur de l'actionneur
 * buffer - la chaîne de caractères représentant la trame ainsi créée
 */
void convertToFrame(idValue idValue, char* buffer);


#endif
