


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
#include "Message.h"

int main(void)
{

//    char* buffer = malloc(FRAME_SIZE_ENOCEAN);
    //SOCKET sock = socketConnexion();
  //  tcpFrameType myFrame = tcpFrameCreation(buffer);
   // enOceanMessage myMessage = enOceanMessageCreation(myFrame);
    //sendSensorStateByPipe(myMessage);
    
    /*while (sock != SOCKET_ERROR)
    {
        socketFrameReception(sock,buffer);
        tcpFrameType myFrame = tcpFrameCreation(buffer);
        enOceanMessage myMessage = enOceanMessageCreation(myFrame);
        sendSensorStateByPipe(myMessage);
        
    } */  
   // free(buffer);
   // char* buffer = malloc(FRAME_SIZE_ENOCEAN);
  //  SOCKET sock = socketConnexion();
   // while (sock != SOCKET_ERROR)
   // {
   //     socketFrameReception(sock,buffer);
    //    tcpFrame myFrame = tcpFrameCreation(buffer);
   //     //enOceanMessage myMessage = enOceanMessageCreation(myFrame);

        
  //  }
  //  free(buffer);

    char * testBuffer= "A55A0B0570000000001F607330A2";
    //Transformation de la trame de char en int
    struct tcpFrame testFrame = tcpFrameCreation(testBuffer);
    printf("DataByte0 %d\n",testFrame.DATA_BYTE0);
    printf("DataByte1 %d\n",testFrame.DATA_BYTE1);
    printf("DataByte2 %d\n",testFrame.DATA_BYTE2);
    printf("DataByte3 %d \n",testFrame.DATA_BYTE3);
    printf("IDByte0 %c\%c\n",testFrame.ID_BYTE0[0],testFrame.ID_BYTE0[1]);
    printf("IDByte1 %c\%c\n",testFrame.ID_BYTE1[0],testFrame.ID_BYTE1[1]);
    printf("IDByte2 %c\%c\n",testFrame.ID_BYTE2[0],testFrame.ID_BYTE2[1]);
    printf("IDByte3 %c\%c\n",testFrame.ID_BYTE3[0],testFrame.ID_BYTE3[1]);

    //Transformation de la tcpFrame en message
    struct enOceanMessage lemessage = enOceanMessageCreation(testFrame);
    printf("sensorID %s\n",lemessage.sensorID);
    printf("data0 %d\n",lemessage.value0);
    printf("data1 %d\n",lemessage.value1);
    printf("data2 %d\n",lemessage.value2);
    printf("data3 %d\n",lemessage.value3);
    printf("succes");
    return 0;
}

