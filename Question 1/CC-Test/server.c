#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define SERVER_PORT 2012

//Handle_client function which handles clients
void handle_client(int sock, struct sockaddr_in client_addr, socklen_t client_addr_len);
int main() {
    //Creating a socket using the socket function
    //Sock: Socket file descriptor
    //AF_INET: Address family used for the socket in this case IPv4
    //SOCK_DGRAM: The type of socket to create in this case a datagram socket since it is connectionless
    //0: The protocol to use this case, the default protocol for the given address family and socket type
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //Creating a server_addr struct that will store information about the server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    //The address family used for the socket
    server_addr.sin_family = AF_INET;
    //Converts a port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    //Binding the socket to the server information using the bind function
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Server ready to receive from port %d...\n", SERVER_PORT);

    //Infinite loop to handle client connections
    while (1) {
        //Creating a client_addr struct that will store information about the client
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        //Creating child processes to handle clients
        pid_t child_pid = fork();
        //child process
        if (child_pid == 0) {
            handle_client(sock, client_addr, client_addr_len);
            //close socket file descriptor
            close(sock);
            exit(EXIT_SUCCESS);
        }
    }
    close(sock);
    return 0;
}

//Handle client function
void handle_client(int sock, struct sockaddr_in client_addr, socklen_t client_addr_len) {
    //Receive data from the client using the recvfrom function and storing it in client_data variable
    char client_data[100];
    int n = recvfrom(sock, client_data, sizeof(client_data), 0, (struct sockaddr *) &client_addr, &client_addr_len);
    if (n < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    client_data[100] = '\0';
    //Process client request
    // Preventing duplicate records
    //Extract serial_number and registration number from client_data
    char ser[20];
    char reg[20];
    sscanf(client_data,"%s\t%s",ser,reg);
    // Open the file in read mode 
    FILE* fp = fopen("registration.txt", "r");
    // Check if the file was opened successfully
    if (fp == NULL) {
        printf("Error opening file!");
    }

    // Read each line from the file and check if received record already exists
    char line[100];
    while (fgets(line, 100, fp)) {
    	//char line_details variable that will hold each line in the file that will be used to compare with client_data;
        char line_serial_number[20];
        char line_registration_number[20];
        sscanf(line, "%s\t%s", line_serial_number, line_registration_number);
        if (strcmp(line_serial_number, ser) == 0 || strcmp(line_registration_number, reg) == 0) {
        	//Record already exists
        	//close the file
        	fclose(fp);
			//Formulate and send reply to the client informing client that their record already exists
			int j = sendto(sock, "SerialNo or RegNo already exists!!!", strlen("SerialNo or RegNo already exists!!!"), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
			if (j < 0) {
				perror("sendto");
				exit(EXIT_FAILURE);
    		}
    		close(j);
			//close the socket file descriptor
			close(sock);
        }
    }
    // Close the file
    fclose(fp);
    
    //Open the file in append mode
    fopen("registration.txt", "a");
    // Check if the file was opened successfully
    if (fp == NULL) {
        printf("Error opening file!");
    }

    //Write the contents of what was received from the client into the file
    fprintf(fp, "%s\n", client_data);

    //Close the file
    fclose(fp);

    //Send response of success to the client
    int j = sendto(sock, "Records have been saved successfully", strlen("Records have been saved successfully"), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (j < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    close(j);
}
