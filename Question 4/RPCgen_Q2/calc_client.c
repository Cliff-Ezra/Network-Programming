/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calc.h"


void
calc_prog_1(char *host,float x,float y, int z)
{
	CLIENT *clnt;
	float  *result_1;
	input  calc_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, CALC_PROG, ADD_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	calc_1_arg.a = x;
	calc_1_arg.b = y;
	calc_1_arg.c = z;
	result_1 = calc_1(&calc_1_arg, clnt);
	if (result_1 == (float *) NULL) {
		clnt_perror (clnt, "call failed");
	}else{
		printf("Result: %f\n",*result_1);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	printf("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n");

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	calc_prog_1 (host,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
exit (0);
}
