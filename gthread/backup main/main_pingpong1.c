#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1


typedef void (func_t)(void *);
typedef struct
{
	int* esp;
	int* ebp;
	func_t* function;
	void* args;
	void* pileExecution;
}ctx_s;

ctx_s ctx_ping;
ctx_s ctx_pong;
ctx_s* ctx_courant;
void f_ping(void *arg);
void f_pong(void *arg);


int init_ctx(ctx_s* ctx, int stack_size,func_t f, void* args);
void switch_to_ctx(ctx_s *ctx);

int init_ctx(ctx_s *ctx, int stack_size,func_t f, void *args)
{
	ctx->pileExecution = malloc(stack_size);
	ctx->esp = malloc(sizeof(int));
	ctx->ebp = malloc(sizeof(int));
	*(ctx->esp) = 0;
	*(ctx->ebp) = 0;
	ctx->function=f;
	ctx->args = args;
	return 0;
}
/*
int init_courant_ctx()
{
	ctx_courant.esp = malloc(sizeof(int));
	ctx_courant.ebp = malloc(sizeof(int));
	return 0;
}
*/

void switch_to_ctx(ctx_s *ctx)
{
	int x, y;
	__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(y),"=r"(x) 
			:  
			: "%esp","%ebp"); 
	
	printf("X : %d\n",x);
	printf("Y : %d\n",y);
	
	if(ctx_courant!=NULL)
	{
		*(ctx_courant->esp) = x;
		*(ctx_courant->ebp) = y;
	}
	ctx_courant=ctx;
	if(*(ctx->esp) == 0)
	{
		ctx->function(ctx->args);
	}
	else
	{
		printf("esp : %d\n",*(ctx->esp));
		printf("ebp : %d\n",*(ctx->ebp));
		x=*(ctx->esp);
		y=*(ctx->ebp);
		/*
		__asm__("movl %%ss,%0" 
				: 
				: "r"(z)
				: "%ss"); 
		printf("-*ss*- : %d\n",z);*/
		__asm__("movl %1,%%esp\n\t" 
				"movl %0,%%ebp "
				: 
				: "r"(y),"r"(x)  
				: "%esp","%ebp"); 
	}
	printf("TEST");
	return;
}

int main(int argc, char *argv[])
{
	printf("PING-PONG");
	ctx_courant=NULL;
	init_ctx(&ctx_ping, 16384, f_ping, NULL);
	init_ctx(&ctx_pong, 16384, f_pong, NULL);
	switch_to_ctx(&ctx_ping);
	exit(EXIT_SUCCESS);
}
void f_ping(void *args)
{
	while(1) {
		printf("A") ;
		switch_to_ctx(&ctx_pong);
		printf("B") ;
		switch_to_ctx( &ctx_pong);
		printf("C") ;
		switch_to_ctx(&ctx_pong);
	}
}
void f_pong(void *args)
{
	while(1) {
		printf("1") ;
		switch_to_ctx(&ctx_ping);
		printf("2") ;
		switch_to_ctx(&ctx_ping);
	}
}

