//
//  main.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"
#include "Listener.h"

int lancerActionneur (int argc, const char * argv[]){
//TODO: not implemented	
return 0;
}


int main (int argc, const char * argv[])
{
    printf("Bonjour\n");
    char* buffer = malloc(BUFFER_RECEIVE_SIZE);
    //tcpFrameWithServerMessageWithBuffer("DO ABCDE 1");
    idValue temp = parseBuffer("DO ABCDE 1");
    printf("%s\n",temp.ID);
    printf("%c\n",temp.value);
    free(buffer);
}

