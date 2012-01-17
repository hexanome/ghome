
typedef unsigned int socklen_t;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;


#include <stdio.h>
#include <stdlib.h>
#define PORT 5000
#define ADRESSE "127.0.0.1"

/*
 Connection to iftpserv2.insa-lyon.fr closed.
 wifinsa396:modulesTraduction alexandrelefoulon$ ssh -L 5000:134.214.105.28:5000 alefoulon@iftpserv2.insa-lyon.fr 
 alefoulon@iftpserv2.insa-lyon.fr's password: 
 Last login: Tue Jan 10 11:33:44 2012 from wifinsa396.insa-lyon.fr
 ------------------------------------------------------------------
 Bienvenue sur :
 _    ___                                   ______
 (_)  / __)  _                              (_____ \
 _ _| |__ _| |_ ____   ___ _____  ____ _   _ ____) )
 | (_   __|_   _)  _ \ /___) ___ |/ ___) | | / ____/
 | | | |    | |_| |_| |___ | ____| |    \ V / (_____
 |_| |_|     \__)  __/(___/|_____)_|     \_/|_______) .insa-lyon.fr
 |_|
 
 (ASCII Art by http://www.network-science.de/ascii)
 ------------------------------------------------------------------
 telnet localhost 5000
 */

int main(void)
{

    int erreur = 0;
  
    SOCKET sock;
    SOCKADDR_IN sin;
    
    if(!erreur)
    {
        /* Création de la socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        /* Configuration de la connexion */
        sin.sin_addr.s_addr = inet_addr(ADRESSE);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);
        
        /* Si le client arrive à se connecter */
        if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
            printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        else
            printf("Impossible de se connecter\n");
        
        /* On ferme la socket précédemment ouverte */
        closesocket(sock);
        
    }
    
    return EXIT_SUCCESS;
}