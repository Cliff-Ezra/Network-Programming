static char ibmcopyr[] =
    "GENESEND - Licensed Materials - Property of IBM. "
    "This module is \"Restricted Materials of IBM\" "
    "5647-A01 Copyright IBM Corp. 2013. "
    "See IBM Copyright Instructions.";

#define MVS
#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>

#define intrcvprog ((u_long)150000)
#define version ((u_long)1)
#define intrcvproc ((u_long)1)

main(argc, argv) int argc;
char *argv[];
{
    int innumber;
    int outnumber;
    int error;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s hostname integer\n", argv[0]);
        exit(-1);
    } /* endif */
    innumber = atoi(argv[2]);
    /*
     * Send the integer to the server. The server should
     * return the same integer.
     */
    error = callrpc(argv[1], intrcvprog, version, intrcvproc, xdr_int,
                    (char *)&innumber, xdr_int, (char *)&outnumber);

    if (error != 0)
    {
        fprintf(stderr, "error: callrpc failed: %d \n", error);
        fprintf(stderr, "intrcprog: %d version: %d intrcvproc: %d",
                intrcvprog, version, intrcvproc);
        exit(1);
    } /* endif */

    printf("value sent: %d   value received: %d\n", innumber, outnumber);
    exit(0);
}