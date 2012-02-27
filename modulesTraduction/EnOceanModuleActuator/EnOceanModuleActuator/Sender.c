//
//  Sender.c
//  EnOceanModuleActuator
//
//  Created by Alexandre Lefoulon on 31/01/12.
//  Copyright (c) 2012 INSA Lyon. All rights reserved.
//

#include <stdio.h>
#include "Sender.h"

/**
 * Crée une socket sur l'adresse ADRESSE et le port PORT défini dans Sender.h
 */
SOCKET socketConnexionActuator(void)
{
    SOCKET sock;
    SOCKADDR_IN sin;
    
	/* Création de la socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	/* Configuration de la connexion */
	sin.sin_addr.s_addr = inet_addr(ADRESSE);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	
	/* Si le client arrive à se connecter */
	if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR){
		printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
		return sock;}
	else{
		printf("Impossible de se connecter\n");
    		return -1;}
}

/**
 * Lit dans le fichier fifo qui permet de faire la communication avec le serveur
 * le message de l'action à faire (de la forme "DO 0021CBE5 1")
 * et envoie la trame correspondante à la station
 * pipe - le fichier fifo
 * sock - la socket pour l'envoie des messages
 */
void pipeReceiveSocketSend (FILE* pipe, SOCKET sock) {
    char* bufferPipe = malloc(BUFFER_RECEIVE_SIZE);
    char* bufferFrame = malloc(BUFFER_RECEIVE_SIZE);
    char* result;
    //buffer = "DO FF9F1E03 1";
    idValue envoi;
    
    //while (getline(&bufferPipe, BUFFER_RECEIVE_SIZE, pipe)) {
//while (fscanf(pipe,"%s",&bufferPipe)) {
//fscanf(pipe,"%d",value0);
//fscanf(pipe,"%d",value1);
     while (1) {   
        //result = fgets(bufferPipe, BUFFER_RECEIVE_SIZE, pipe);
	//printf("boucle\n");
	/*if(result!=NULL)
	{
		envoi = parseBuffer(bufferPipe); ;
		convertToFrame(envoi, bufferFrame);
		if(send(sock, bufferFrame, strlen(bufferFrame), 0) < 0)
		{
		    printf("Erreur d'envoi de la trame d'actionneur");
		}  
	}*/
    }
    free(bufferPipe);
    free(bufferFrame);
    return;
    
}







