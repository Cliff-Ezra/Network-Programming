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

int main()
{
    int *intrcv();

    registerrpc(intrcvprog, intvers, intrcvproc, intrcv, xdr_input_data, xdr_int);
    printf("Intrcv Registration with Port Mapper completed\n");

    svc_run();
    printf("Error:svc_run returned!\n");
    exit(1);
}

int *intrcv(struct {
    int num1;
    int num2;
    int operation;
} * in)
{
    static int result;
    // print the values received
    printf("num1: %d\n", in->num1);
    printf("num2: %d\n", in->num2);
    printf("operation: %d\n", in->operation);

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
        fprintf(stderr, "Invalid operation\n");
        exit(1);
        break;
    }

    return &result;
}
