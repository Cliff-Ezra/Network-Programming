#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>

#define intrcvprog ((u_long)150000)
#define version ((u_long)1)
#define intrcvproc ((u_long)1)

bool_t xdr_input_data(
    XDR *xdrs, struct {
        int num1;
        int num2;
        int operation;
    } * data)
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

    printf("Enter num1: ");
    scanf("%d", &num1);

    printf("Enter num2: ");
    scanf("%d", &num2);

    printf("Select an operation:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    scanf("%d", &operation);

    struct
    {
        int num1;
        int num2;
        int operation;
    } indata = {num1, num2, operation};

    error = callrpc(argv[1], intrcvprog, version, intrcvproc, xdr_input_data, (char *)&indata, xdr_int, (char *)&result);
    if (error != 0)
    {
        fprintf(stderr, "error: callrpc failed: %d \n", error);
        fprintf(stderr, "intrcprog: %d version: %d intrcvproc: %d", intrcvprog, version, intrcvproc);
        exit(1);
    }

    printf("Result: %d\n", result);
    exit(0);
}
