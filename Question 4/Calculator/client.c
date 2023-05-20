#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>

#define INTRCVPROG ((u_long)150000)
#define VERSION ((u_long)1)
#define INTRCVPROC ((u_long)1)

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

int main(int argc, char *argv[])
{
    int num1, num2;
    int operation;
    int error;
    int result;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        exit(-1);
    }

    // Get user input for numbers and operation
    printf("Enter num1: ");
    scanf("%d", &num1);

    printf("Enter num2: ");
    scanf("%d", &num2);

    printf("\n");
    printf("*********************************************\n");
    printf("\tSelect an operation:\n");
    printf("\t1. Addition (+)\n");
    printf("\t2. Subtraction(-)\n");
    printf("\t3. Multiplication (*)\n");
    printf("\t4. Division (/)\n");
    printf("*********************************************\n");
    scanf("%d", &operation);

    struct input_data indata = {num1, num2, operation};

    // Call remote procedure on server
    error = callrpc(argv[1], INTRCVPROG, VERSION, INTRCVPROC,
                    xdr_input_data, (char *)&indata,
                    xdr_int, (char *)&result);
    if (error != 0)
    {
        fprintf(stderr, "error: callrpc failed: %d ⛔\n", error);
        fprintf(stderr, "INTRCPROG: %d VERSION: %d INTRCVPROC: %d",
                INTRCVPROG, VERSION, INTRCVPROC);
        exit(1);
    }

    // Print the result
    printf("Result received from server📩: %d\n", result);
    exit(0);
}
