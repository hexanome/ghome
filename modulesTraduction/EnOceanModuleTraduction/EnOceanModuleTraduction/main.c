


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


#include "Listener.h"


int main (void) {
    
    char* buffer = malloc(FRAME_SIZE_ENOCEAN);
    SOCKET sock = socketConnexion();
    while (sock != SOCKET_ERROR) {
    socketFrameReception(sock,buffer);
    }   
    free(buffer);
}