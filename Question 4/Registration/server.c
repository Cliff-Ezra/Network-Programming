#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>

#define strrcvprog ((u_long)150000)
#define strvers ((u_long)1)
#define strrcvproc ((u_long)1)

struct input
{
    int serial_num;
    char *reg_num;
    char *name;
};

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

struct output
{
    int status;
};

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
    /* REGISTER PROG, VERS AND PROC WITH THE PORTMAPPER */
    registerrpc(strrcvprog, strvers, strrcvproc, strrcv,
                (xdrproc_t)xdr_input, (xdrproc_t)xdr_output);
    printf("Strrcv Registration with Port Mapper completed\n");

    /* svc_run will handle all requests for programs registered. */
    svc_run();
    printf("Error:svc_run returned!\n");
    exit(1);
}

/* Procedure called by the server to receive and return a string. */
struct output *strrcv(struct input *in)
{
    static struct output out;
    FILE *fp;
    int found = 0;
    int serial_num;
    char reg_num[512];
    char name[512];

    printf("serial number received: %d\n", in->serial_num);
    printf("registration number received: %s\n", in->reg_num);
    printf("name received: %s\n", in->name);

    fp = fopen("Students.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s\n", &serial_num, reg_num, name) != EOF)
        {
            if (serial_num == in->serial_num || strcmp(reg_num, in->reg_num) == 0)
            {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (!found)
    {
        fp = fopen("Students.txt", "a");
        fprintf(fp, "%d\t\t %s\t\t %s\t\t\n", in->serial_num, in->reg_num, in->name);
        fclose(fp);
        out.status = 0; // success
    }
    else
    {
        out.status = 1; // failure
    }

    return &out;
}
