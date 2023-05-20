/* Client code */
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRRCVPROG ((u_long)150000)
#define VERSION ((u_long)1)
#define STRRCVPROC ((u_long)1)

// Define input data structure
struct input
{
    int serial_num;
    char *reg_num;
    char *name;
};

// Function to encode/decode input data
bool_t xdr_input(XDR *xdrs, struct input *objp)
{
    if (!xdr_int(xdrs, &objp->serial_num))
    {
        return (FALSE);
    }
    if (!xdr_string(xdrs, &objp->reg_num, 512))
    {
        return (FALSE);
    }
    if (!xdr_string(xdrs, &objp->name, 512))
    {
        return (FALSE);
    }
    return (TRUE);
}

// Define output data structure
struct output
{
    int status;
};

// Function to encode/decode output data
bool_t xdr_output(XDR *xdrs, struct output *objp)
{
    if (!xdr_int(xdrs, &objp->status))
    {
        return (FALSE);
    }
    return (TRUE);
}

int main(int argc, char *argv[])
{
    struct input instrs;
    struct output outstrs;
    int error;

    if (argc != 2)
    {
        fprintf(stderr, "usage:%s hostname\n", argv[0]);
        exit(-1);
    }

    instrs.reg_num = (char *)malloc(512);
    instrs.name = (char *)malloc(512);

    // Get user input for student details
    printf("Enter serial number: ");
    scanf("%d", &instrs.serial_num);

    printf("Enter registration number: ");
    scanf("%s", instrs.reg_num);

    printf("Enter name: ");
    getchar();
    fgets(instrs.name, 512, stdin);
    instrs.name[strcspn(instrs.name, "\n")] = 0;

    // Call remote procedure on server
    error = callrpc(argv[1], STRRCVPROG,
                    VERSION,
                    STRRCVPROC,
                    (xdrproc_t)xdr_input,
                    (char *)&instrs,
                    (xdrproc_t)xdr_output,
                    (char *)&outstrs);

    if (error != 0)
    {
        fprintf(stderr, "error: callrpc failed:%d ⛔\n", error);
        fprintf(stderr, "STRCPROG:%d VERSION:%d STRCVPROC:%d",
                STRRCVPROG,
                VERSION,
                STRRCVPROC);

        exit(1);
    }

    // Print the result
    if (outstrs.status == 0)
    {
        printf("Student added successfully✅\n");
    }
    else
    {
        printf("Error adding student. Serial number or registration number already exists.⛔\n");
    }

    free(instrs.reg_num);
    free(instrs.name);

    exit(0);
}
