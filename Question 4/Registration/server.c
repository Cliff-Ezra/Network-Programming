/* Server code */
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>

#define STRRCVPROG ((u_long)150000)
#define STRVERS ((u_long)1)
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

struct output *strrcv(struct input *in);

int main()
{
    // Register RPC program with port mapper
    registerrpc(STRRCVPROG, STRVERS, STRRCVPROC, strrcv,
                (xdrproc_t)xdr_input, (xdrproc_t)xdr_output);
    printf("Strrcv Registration with Port Mapper completed✅\n");

    // Start RPC service
    svc_run();
    printf("Error:svc_run returned!⛔\n");
    exit(1);
}

// Function to handle incoming RPC requests
// Function to handle incoming RPC requests
struct output *strrcv(struct input *in)
{
    static struct output out;
    FILE *fp;
    int found = 0;
    int serial_num;
    char reg_num[512];
    char name[512];

    // Print the values received
    printf("serial number received: %d\n", in->serial_num);
    printf("registration number received: %s\n", in->reg_num);
    printf("name received: %s\n", in->name);

    // Check if student already exists in file
    fp = fopen("Students.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %[^\n] %[^\n]\n", &serial_num, reg_num, name) != EOF)
        {
            if (serial_num == in->serial_num || strcmp(reg_num, in->reg_num) == 0)
            {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    // If student does not exist in file, add them
    if (!found)
    {
        fp = fopen("Students.txt", "a");
        fprintf(fp, "%d\t\t %s\t\t %s\n", in->serial_num, in->reg_num, in->name);
        fclose(fp);
        out.status = 0; // success
        printf("Student added successfully✅\n");
    }
    else
    {
        out.status = 1; // failure
        printf("Error adding student. Serial number or registration number already exists.⛔\n");
    }

    printf("Returning result to client📨\n");
    return &out;
}
