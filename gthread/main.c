#include <stdio.h>
#include <stdlib.h>
#include "gthread.h"

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *args);

int main(int argc, char *argv[])
{
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	create_ctx(16384, f_pang, NULL);
	start_sched();
	exit(EXIT_SUCCESS);
}
void f_ping(void *args)
{
	while(1) {
		printf("A") ;
		printf("B") ;
		printf("C") ;
	}
}
void f_pong(void *args)
{
	while(1) {
		printf("1") ;
		printf("2") ;
	}
}
void f_pang(void *args)
{
	while(1) {
		printf("*") ;
		printf("-") ;
	}
}

