#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1


typedef void (func_t)(void *);

int init_ctx(struct ctx_s *ctx, int stack_size,func_t f, void *args);
void switch_to_ctx(struct ctx_s *ctx);


/*struct ctx
{
	jmp_buf buf;
	func_t function;
	void* args;
	void* pileExecution
	int firstCall;
};*/
struct ctx
{
	int* esp;
	int* ebp;
	func_t function;
	void* args;
	void* pileExecution
}



static ctx ctx_courant;
int init_ctx(struct ctx_s *ctx, int stack_size,func_t f, void *args)
{
	int return_value=0;
	pileExecution = malloc(stack_size);
	ctx->esp = NULL;
	ctx->ebp = NULL;
	ctx->function=f
	ctx->args = args;
	return 0;
}

void switch_to_ctx(struct ctx_s *ctx)
{
	unsigned long x, y;
	__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(y),"=r"(x) //y is output operand
			:  //there is no input
			: "%esp","%ebp"); // %esp and %ebp are use in this function 
	ctx_courant->epb* = x;
	ctx_courant->esp* = y;
	
	if(ctx->esp == null)
	{
		ctx->firstCall =1;
		ctx_courant = ctx;
		ctx->function(ctx->args);
	}
	else
	{
		__asm__("movl %1,%%esp\n\t" 
				"movl %0,%%ebp, "
				: "=r"(y),"=r"(x) //y is output operand
				:  //there is no input
				: "%esp","%ebp"); // %esp and %ebp are use in this function 
	}
	return;
}
/*
int throw(struct ctx_s *pctx, int r)
{
	longjmp(pctx->buf,r);
}

int try(struct ctx_s *pctx, func_t *f, int arg)
{
	int i;
	i=setjmp(pctx->buf)
	if(!i)
	{
		return func_t(arg);
	}
	else
	{
		return i;
	}	
}*/

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char *argv[])
{
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
		switch_to_ctx(&ctx_pong);
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



/*
int
main()
{	
	int product;

	unsigned long x, y;
	printf("@x = %d\n", &x);
	__asm__("movl %%esp, %1\n\t" 
				"movl %%ebp, %0"
				: "=r"(y),"=r"(x) // y is output operand 
				:  // x is input operand 
				  : "%esp","%ebp"); // %eax is a clobbered register
		printf("x ( esp )  = %d\n", x);
		printf("y ( ebp )  = %d\n", y);
	printf("A list of int, please\n");
	product = mul(0);
	printf("product = %d\n", product);


	__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(y),"=r"(x) //y is output operand
			:  //x is input operand 
			  : "%esp","%ebp"); // %eax is a clobbered register 
	printf("x ( esp )  = %d\n", x);
	printf("y ( ebp )  = %d\n", y);
}
*/

/*int
main(int argc, char *argv[])
{
    create_ctx(16384, f_pong, NULL);
    create_ctx(16384, f_ping, NULL);
    start_sched();
    exit(EXIT_SUCCESS);
}*/
