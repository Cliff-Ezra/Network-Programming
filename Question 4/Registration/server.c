static char ibmcopyr[] =
    "GENESERV - Licensed Materials - Property of IBM. "
    "This module is \"Restricted Materials of IBM\" "
    "5647-A01  Copyright IBM Corp. 2013. "
    "See IBM Copyright Instructions.";

#ifndef MVS
#define MVS
#endif
#include <rpc/rpc.h>
#include <stdio.h>

#define intrcvprog ((u_long)150000)
#define fltrcvprog ((u_long)150102)
#define intvers ((u_long)1)
#define intrcvproc ((u_long)1)
#define fltrcvproc ((u_long)1)
#define fltvers ((u_long)1)

main()
{
    int *intrcv();
    float *floatrcv();

    /*REGISTER PROG, VERS AND PROC WITH THE PORTMAPPER*/

    /*FIRST PROGRAM*/
    registerrpc(intrcvprog, intvers, intrcvproc, intrcv, xdr_int, xdr_int);
    printf("Intrcv Registration with Port Mapper completed\n");

    /*OR MULTIPLE PROGRAMS*/
    registerrpc(fltrcvprog, fltvers, fltrcvproc,
                floatrcv, xdr_float, xdr_float);
    printf("Floatrcv Registration with Port Mapper completed\n");

    /*
     * svc_run will handle all requests for programs registered.
     */
    svc_run();
    printf("Error:svc_run returned!\n");
    exit(1);
}

/*
 * Procedure called by the server to receive and return an integer.
 */
int *intrcv(in)
int *in;
{
    int *out;

    printf("integer received: %d\n", *in);
    out = in;
    printf("integer being returned: %d\n", *out);
    return (out);
}

/*
 * Procedure called by the server to receive and return a float.
 */

float *
floatrcv(in)
float *in;
{
    float *out;

    printf("float received: %e\n", *in);
    out = in;
    printf("float being returned: %e\n", *out);
    return (out);
}