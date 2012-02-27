/* ------------------------------
   $Id: hw_serialline.c 97 2009-10-15 17:26:01Z simon_duquennoy $
   ------------------------------------------------------------

   hw_serialline.c 

   Module de simulation des liens serie. 

*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <time.h>

#include "hardware.h"
#include "hw_tools.h"
#include "hw_serialline.h"

/**
 * Constantes spécifiants les registres de configuration du matériel simulé.
 * (initialisée en fonction du fichier de configuration)
 */
int  SL1_ENABLE    = 0 ;
char *SL1          = NULL ;          
int  SL1_COM       = 0 ;
int  SL1_NOISE     = 0 ;
int  SL1_IRQ       = 0 ;
int  SL1_UARTSR    = 0 ;
int  SL1_UARTDATA  = 0 ;
int  SL1_UDPPORT   = 0 ;

int  SL2_ENABLE    = 0 ;
char *SL2          = NULL ;
int  SL2_COM       = 0 ;
int  SL2_NOISE     = 0 ;
int  SL2_IRQ       = 0 ;
int	 SL2_UARTSR    = 0 ;
int  SL2_UARTDATA  = 0 ;
int	 SL2_UDPPORT   = 0 ;

/**
 * variables globales d'émulation des liaisons series simulées
 */
struct serialReg{
	int statusReg;
	int dataReg;
	int datagramSocket;
} ;
struct serialReg *regSL1 = NULL;
struct serialReg *regSL2 = NULL;
struct serialReg *currentSL = NULL;

void SL1IRQFct(int sig);
/**
 * procedure d'initialisation des liaisons series 
 */
int init_Serialline()
{
	int res = 1;
	int rc;
	struct sockaddr_in servAddr;

	trace(DEBUG_SETUP,"trying serialline initialization...\n");
	if(SL1_ENABLE || SL2_ENABLE)
	{ /* alloc shared ressources */

	}

	if(SL1_ENABLE)
	{
		if((regSL1 = (struct serialReg *)getSharedMem(sizeof(struct serialReg)))==NULL)
		{
			trace(DEBUG_WARNING,"Seriallines initialization failed, out of shared mem (EC1)\n");
			return 0;
		}
		trace(DEBUG_SETUP,"serialline 1 enable.\n");
		/* datagram socket creation */
		regSL1->datagramSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if(regSL1->datagramSocket < 0) {
			trace(DEBUG_WARNING,"serial line \"com1\" simulation failed, cannot open datagram socket\n");
			res = 0;
		} else {
			/* bind local server port */
			servAddr.sin_family = AF_INET;
			servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			servAddr.sin_port = htons(SL1_UDPPORT);
			rc = bind (regSL1->datagramSocket, (struct sockaddr *) &servAddr,sizeof(servAddr));
			if(rc<0) {
				trace(DEBUG_WARNING,"serial line \"com1\" simulation failed, cannot bind SL1_UDPPORT=%d\n", SL1_UDPPORT);
				res = 0;
			} else {
				int ppid;
				char buffer[3];
				fd_set fds;
				struct timeval timeout;
				timeout.tv_sec  = 1;
				timeout.tv_usec = 0;
				FD_ZERO(&fds);
				FD_SET(regSL1->datagramSocket,&fds);
				/* start a child process for incoming datas */
				switch(fork()){
					case 0:
						/* manage incoming datas */
						srand( (unsigned)time( NULL ) );
						while((ppid = getppid()) != 1){
							if(!select(1,&fds,NULL,NULL,&timeout))
							{
								rc = recvfrom( regSL1->datagramSocket , buffer, 3, 0, NULL, 0 );
								/* check incoming config */
								if( (regSL1->statusReg & 0x7) != (((int)buffer[1]) & 0x07) )
								{
									trace(DEBUG_WARNING," Wrong received Rate on SL1... \n");
									regSL1->statusReg &= 0xF3FF ;
									regSL1->statusReg |= 0x0500 ;
									regSL1->dataReg = rand() & 0xFF ;
									sendSignal(0xC0DE0001);
								}
								else if( (regSL1->statusReg & 0x18) != (((int)buffer[1]) & 0x18) )
								{
									trace(DEBUG_WARNING," Wrong parity bits configuration on SL1... \n");
									regSL1->statusReg &= 0xF3FF ;
									regSL1->statusReg |= 0x0500 ;
									regSL1->dataReg = rand() & 0xFF ;
								}
								else
								{
									if((rand()%1000)>=SL1_NOISE)
									{
										regSL1->statusReg &= 0xF3FF ;
										regSL1->statusReg |= 0x0D00 ;
										regSL1->dataReg = ((int)buffer[2]) & 0xFF ;
									}
									else
									{
										trace(DEBUG_WARNING,"Noise on SL1\n");
										regSL1->statusReg &= 0xF3FF ;
										regSL1->statusReg |= 0x0500 ;
										regSL1->dataReg = (buffer[2]^(1<<(rand()&8))) & 0xFF ;
									}
									sendSignal(0xC0DE0001);
								}
							}
							else
							{
								trace(DEBUG_WARNING,"select wrong case...\n");
							}
						}
						trace(DEBUG_WARNING,"SerialLine \"COM1\" simulation terminated...\n");
						exit(0);
						break;
					case -1:
						trace(DEBUG_WARNING,"serial line \"com1\" simulation failed, fork failed.\n");
						res = 0;
						break;
					default:
						addSigHandler(0xC0DE0001,SL1IRQFct);
						break;
				}
			}
			if(res)
				trace(DEBUG_SETUP,"serial line \"com1\" simulation ready.\n");
		}
	}
	/* Initialisation du port com 2 */

	if(SL2_ENABLE)
	{
		if((regSL2 = (struct serialReg *)getSharedMem(sizeof(struct serialReg)))==NULL)
		{
			trace(DEBUG_WARNING,"Seriallines initialization failed, out of shared mem (EC2)\n");
			return 0;
		}
		/* datagram socket creation */
		regSL2->datagramSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if(regSL2->datagramSocket < 0) 
		{
			trace(DEBUG_WARNING,"serial line \"com2\" simulation failed, cannot open datagram socket\n");
			res = 0;
		}	else {
			/* bind local server port */
			servAddr.sin_family = AF_INET;
			servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			servAddr.sin_port = htons(SL2_UDPPORT);
			rc = bind (regSL2->datagramSocket, (struct sockaddr *) &servAddr,sizeof(servAddr));
			if(rc<0) {
				trace(DEBUG_WARNING,"serial line \"com2\" simulation failed, cannot bind SL2_UDPPORT=%d\n", SL1_UDPPORT);
				res = 0;
			} else {
				trace(DEBUG_SETUP,"serial line \"com2\" simulation ready.\n");
			}
		}
	}

  if(res)
    trace(DEBUG_SETUP,"Seriallines initialization completed.\n");
	else
    trace(DEBUG_SETUP,"Seriallines initialization failed.\n");

	return res;
}

int in_SL1_UARTSR(int port)
{
	int t = regSL1->statusReg ;
	regSL1->statusReg &= 0xF0FF ;
	return t ;
}

int in_SL1_UARTDATA(int port)
{
	return regSL1->dataReg ;
}

static int rateValue[8]={1200,2400,4800,9600,14400,28800,38600,56000};

void out_SL1_UARTSR(int port, int value)
{
	trace(DEBUG_HWREG,"SL1 Status Register\n");
	if(value&0x08)
		trace(DEBUG_HWREG, ". %s parity\n", (value&0x10)?"Even":"Odd" );
	else
		trace(DEBUG_HWREG, ". No Parity\n");
	trace(DEBUG_HWREG,". Rate %d Bauds\n",rateValue[value&0x7]);
	regSL1->statusReg = (regSL1->statusReg & ~0xFF) | (value & 0xFF) ;
}

void genIT()
{
	regSL1->statusReg |= 0x0300 ;
	sendSignal(0xC0DE0001);
}

void out_SL1_UARTDATA(int port, int value)
{
	char buffer[3];
	int rc;
	struct sockaddr_in remoteServAddr;
	struct hostent *h;

	buffer[0] = (char) (0) ;
	buffer[1] = (char) (regSL1->statusReg & 0xFF) ;
	buffer[2] = (char) value ;
	h = gethostbyname(SL1);
	if(h==NULL) {
		trace(DEBUG_WARNING,"Warning %s machine is unknown \n",SL1);
		return ;
	}
	remoteServAddr.sin_family = h->h_addrtype;
	memcpy((char *)	&remoteServAddr.sin_addr.s_addr, 
					h->h_addr_list[0], 
					h->h_length);
	remoteServAddr.sin_port = htons(SL1_UDPPORT);

/*	trace(DEBUG_WARNING,"send '%c' to \"%s\" [%d]!\n",c,SL1,SL1_UDPPORT); */
	rc = sendto( regSL1->datagramSocket , buffer, 3, 0, 
			(struct sockaddr *) &remoteServAddr, 
			sizeof(remoteServAddr));
	if(rc < 0) {
		trace(DEBUG_WARNING,"send failed ! \n");
	}
	addTickHandler(2, 0, genIT) ;
}

/**
  
 */
void SL1IRQFct(int sig) {

	processUserIRQ(SL1_IRQ);
	
	return ;
}

