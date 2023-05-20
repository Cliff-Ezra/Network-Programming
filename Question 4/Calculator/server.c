/* Server Code */
#ifndef MVS
#define MVS
#endif

#include <rpc/rpc.h>
#include <stdio.h>

#define INTRCVPROG ((u_long)150000)
#define FLTRCVPROG ((u_long)150102)
#define INTVERS ((u_long)1)
#define INTRCVPROC ((u_long)1)
#define FLTRCVPROC ((u_long)1)
#define FLTVERS ((u_long)1)

// Define input data structure
struct input_data
{
    int num1;
    int num2;
    int operation;
};

// Function to encode/decode input data
bool_t xdr_input_data(XDR *xdrs, struct input_data *data)
{
    if (!xdr_int(xdrs, &data->num1))
    {
        return FALSE;
    }
    if (!xdr_int(xdrs, &data->num2))
    {
        return FALSE;
    }
    if (!xdr_int(xdrs, &data->operation))
    {
        return FALSE;
    }
    return TRUE;
}

int main()
{
    int *intrcv();

    // Register RPC program with port mapper
    registerrpc(INTRCVPROG, INTVERS, INTRCVPROC, intrcv, xdr_input_data, xdr_int);
    printf("Intrcv Registration with Port Mapper completed✅\n");

    // Start RPC service
    svc_run();
    printf("Error:svc_run returned!⛔\n");
    exit(1);
}

// Function to handle incoming RPC requests
int *intrcv(struct input_data *in)
{
    static int result;

    // Print the values received
    printf("\n");
    printf("*********************************************\n");
    printf("Values received from the client📩\n");
    printf("num1: %d\n", in->num1);
    printf("num2: %d\n", in->num2);
    printf("operation: %d\n", in->operation);
    printf("*********************************************\n");

    // Perform the requested operation
    switch (in->operation)
    {
    case 1:
        result = in->num1 + in->num2;
        break;
    case 2:
        result = in->num1 - in->num2;
        break;
    case 3:
        result = in->num1 * in->num2;
        break;
    case 4:
        result = in->num1 / in->num2;
        break;
    default:
        fprintf(stderr, "Invalid operation⛔\n");
        exit(1);
        break;
    }

    // Return the result
    printf("Result sent to the client📨\n");
    return &result;
}
