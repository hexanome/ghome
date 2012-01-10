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
	int i;
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
			return i * mul(depth+1);
		else
			longjmp(buf,~0);
	}
}

int
main()
{
	int product;
	printf("A list of int, please\n");
	product = mul(0);
	printf("product = %d\n", product);
}


/*int
main(int argc, char *argv[])
{
    create_ctx(16384, f_pong, NULL);
    create_ctx(16384, f_ping, NULL);
    start_sched();
    exit(EXIT_SUCCESS);
}*/
