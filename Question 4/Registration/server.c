#ifndef MVS
#define MVS
#endif

#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>

#define intrcvprog ((u_long)150000)
#define fltrcvprog ((u_long)150102)
#define intvers ((u_long)1)
#define intrcvproc ((u_long)1)
#define fltrcvproc ((u_long)1)
#define fltvers ((u_long)1)

bool_t xdr_input_data(
    XDR *xdrs, struct {
        int serial_number;
        int registration_number;
        char student_name[256];
    } * data)
{
    if (!xdr_int(xdrs, &data->serial_number))
    {
        return FALSE;
    }
    if (!xdr_int(xdrs, &data->registration_number))
    {
        return FALSE;
    }
    if (!xdr_string(xdrs, &data->student_name, 256))
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
    int serial_number;
    int registration_number;
    char student_name[256];
} * in)
{
    static int result;

    FILE *file = fopen("Students.txt", "a+");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file\n");
        exit(1);
    }

    int existing_serial_number;
    int existing_registration_number;
    char existing_student_name[256];
    int serial_number_exists = 0;
    int registration_number_exists = 0;

    while (fscanf(file, "%d %d %s\n", &existing_serial_number, &existing_registration_number, existing_student_name) != EOF)
    {
        if (existing_serial_number == in->serial_number)
        {
            serial_number_exists = 1;
        }
        if (existing_registration_number == in->registration_number)
        {
            registration_number_exists = 1;
        }
        if (serial_number_exists && registration_number_exists)
        {
            break;
        }
    }

    if (serial_number_exists && registration_number_exists)
    {
        result = 3;
    }
    else if (serial_number_exists)
    {
        result = 1;
    }
    else if (registration_number_exists)
    {
        result = 2;
    }
    else
    {
        fprintf(file, "%d %d %s\n", in->serial_number, in->registration_number, in->student_name);
        result = 0;
    }

    fclose(file);

    return &result;
}
