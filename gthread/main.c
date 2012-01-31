#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

#define RUNNING 0
#define READY 1
#define ACTIVATED 2

typedef void (irq_handler_func_t)(void);
#define TIMER_IRQ 2
void setup_irq(unsigned int irq, irq_handler_func_t handler);
void start_hw();
void irq_disable();
void irq_enable();

typedef void (func_t)(void *);
typedef struct
{
	int* esp;
	int* ebp;
	int state;
	func_t* function;
	void* args;
	void* pileExecution;
	void* next;
}ctx_s;

ctx_s* ctx_chainee=NULL;
ctx_s* ctx_courant=NULL;
void f_ping(void *arg);
void f_pong(void *arg);

int create_ctx(int stack_size, func_t f, void *args);
void yield();


void start_sched()
{
	setup_irq(TIMER_IRQ,yield);
	start_hw();
	yield();
}

int create_ctx(int stack_size,func_t f, void *args)
{
	irq_disable();
	ctx_s* ctx = malloc(sizeof(ctx_s));
	if(ctx_chainee!=NULL)
	{
		ctx->next = ctx_chainee->next;
		ctx_chainee->next=ctx;
	}
	else
	{
		ctx_chainee=ctx;
		ctx->next=ctx;
	}
	ctx->pileExecution = malloc(stack_size);
	ctx->esp = ctx->pileExecution+stack_size-1;
	ctx->ebp = ctx->pileExecution+stack_size-1;
	ctx->function=f;
	ctx->args = args;
	ctx->state = READY;
	irq_enable();
	return 0;
}

void start_current_context()
{
	ctx_courant->state=RUNNING;
	irq_enable();
	ctx_courant->function(ctx_courant->args);
}


void yield()
{
	printf("appel!!**--**");
	irq_disable();
	if(ctx_courant!=NULL)
	{
		ctx_courant->state=ACTIVATED;
		__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(ctx_courant->ebp),"=r"(ctx_courant->esp)
			:  
			: "%esp","%ebp");
	}
	ctx_courant=ctx_chainee;
	ctx_chainee=ctx_chainee->next;
	__asm__("movl %1,%%esp\n\t" 
			"movl %0,%%ebp"
			: 
			: "r"(ctx_courant->ebp),"r"(ctx_courant->esp)  
			: "%esp","%ebp"); 
	if(ctx_courant->state == READY)
	{
		start_current_context();
	}
	ctx_courant->state = RUNNING;
	irq_enable();
	return;
}


int main(int argc, char *argv[])
{
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	start_sched();
	exit(EXIT_SUCCESS);
}
void f_ping(void *args)
{
	while(1) {
		/*printf("A") ;
		printf("B") ;
		printf("C") ;*/
	}
}
void f_pong(void *args)
{
	while(1) {
		/*printf("1") ;
		printf("2") ;*/
	}
}

