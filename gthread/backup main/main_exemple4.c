#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

static jmp_buf buf;
static int i = 0;

static int
mul(int depth)
{
	unsigned long x=0, y=0;
	int i;
	printf("@x = %d\n", &x);
	printf("@y = %d\n", &y);

	printf("@depth = %d\n", &depth);
	printf("@i = %d\n", &i);
	__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(y),"=r"(x) /* y is output operand */
			:  /* x is input operand */
			: "%esp","%ebp"); /* %eax is a clobbered register */
	printf("x ( esp )  = %d\n", x);
	printf("y ( ebp )  = %d\n", y);
	
	if(depth==0)
	{
		if(setjmp(buf))
			return 0;
	}

	switch (scanf("%d", &i)) {
	case EOF :
		return 1; /* neutral element */
	case 0 :
		return mul(depth+1); /* erroneous read */
	case 1 :
		if (i)
		{

			return i * mul(depth+1);
		}
		else
			longjmp(buf,~0);
	}
}

int
main()
{	
	int product;

	unsigned long x, y;
	printf("@x = %d\n", &x);
	__asm__("movl %%esp, %1\n\t" 
				"movl %%ebp, %0"
				: "=r"(y),"=r"(x) /* y is output operand */
				:  /* x is input operand */
				  : "%esp","%ebp"); /* %eax is a clobbered register */
		printf("x ( esp )  = %d\n", x);
		printf("y ( ebp )  = %d\n", y);
	printf("A list of int, please\n");
	product = mul(0);
	printf("product = %d\n", product);


	__asm__("movl %%esp, %1\n\t" 
			"movl %%ebp, %0"
			: "=r"(y),"=r"(x) /* y is output operand */
			:  /* x is input operand */
			  : "%esp","%ebp"); /* %eax is a clobbered register */
	printf("x ( esp )  = %d\n", x);
	printf("y ( ebp )  = %d\n", y);
}


/*int
main(int argc, char *argv[])
{
    create_ctx(16384, f_pong, NULL);
    create_ctx(16384, f_ping, NULL);
    start_sched();
    exit(EXIT_SUCCESS);
}*/
