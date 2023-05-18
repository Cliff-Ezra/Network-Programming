#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>

#define intrcvprog ((u_long)150000)
#define version ((u_long)1)
#define intrcvproc ((u_long)1)

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

int main(int argc, char *argv[])
{
    int serial_number;
    int registration_number;
    int error;
    int result;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        exit(-1);
    }

    printf("Enter Serial Number: ");
    scanf("%d", &serial_number);

    printf("Enter Student Registration Number: ");
    scanf("%d", &registration_number);

    char *student_name = malloc(256 * sizeof(char));
    if (student_name == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for student name\n");
        exit(1);
    }

    printf("Enter Student Name: ");
    scanf("%s", student_name);
    // append null character to the end of the string
    student_name[255] = '\0';

    struct
    {
        int serial_number;
        int registration_number;
        char student_name[256];
    } indata = {serial_number, registration_number};

    strcpy(indata.student_name, student_name);

    error = callrpc(argv[1], intrcvprog, version, intrcvproc, xdr_input_data, (char *)&indata, xdr_int, (char *)&result);
    if (error != 0)
    {
        fprintf(stderr, "error: callrpc failed: %d \n", error);
        fprintf(stderr, "intrcprog: %d version: %d intrcvproc: %d", intrcvprog, version, intrcvproc);
        exit(1);
    }

    if (result == 0)
    {
        printf("Student details saved successfully\n");
    }
    else if (result == 1)
    {
        printf("Error: Serial number already exists\n");
    }
    else if (result == 2)
    {
        printf("Error: Registration number already exists\n");
    }
    else if (result == 3)
    {
        printf("Error: Both serial number and registration number already exist\n");
    }

    exit(0);
}