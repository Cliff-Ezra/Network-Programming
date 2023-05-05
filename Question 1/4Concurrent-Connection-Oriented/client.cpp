#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int readSerialNumber(int client_socket);
int readRegistrationNumber(int client_socket);
// Serial Number Program
int main()
{
    // Create a socket and connect to the server
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "Failed to create client socket\n";
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    std::string serial_number;
    std::string registration_number;
    std::string name;

    // Loop until the user chooses to exit
    while (true)
    {
        // Read the serial number from the user
        int serial_number = readSerialNumber(client_socket);
        // check if there was an error
        if (serial_number == 1)
        {
            std::cerr << "Failed to read serial number\n";
            break;
        }

        // Read the registration number from the user
        int registration_number = readRegistrationNumber(client_socket);
        // check if there was an error
        if (registration_number == 1)
        {
            std::cerr << "Failed to read registration number\n";
            break;
        }
        int bytes_sent, bytes_received;
        char buffer[1024];
        std::string response;

        std::cout << "Enter a name: ";
        std::getline(std::cin, name);

        // Send the name to the server
        bytes_sent = send(client_socket, name.c_str(), name.size(), 0);
        if (bytes_sent == -1)
        {
            std::cerr << "Failed to send data to server\n";
            continue;
        }

        // Wait for response from server
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1)
        {
            std::cerr << "Failed to receive data from server\n";
            continue;
        }
        response = std::string(buffer, bytes_received);
        std::cout << response << std::endl;
    }

    // Close the connection
    close(client_socket);

    return 0;
}

// function to read the serail number from the client
int readSerialNumber(int client_socket)
{
    std::string serial_number;
    std::cout << "Enter a serial number (or 'exit' to quit): ";
    std::getline(std::cin, serial_number);

    // Check if the user wants to exit
    if (serial_number == "exit")
    {
        send(client_socket, serial_number.c_str(), serial_number.size(), 0);
        return 0;
    }

    // Send the serial number to the server
    int bytes_sent = send(client_socket, serial_number.c_str(), serial_number.size(), 0);
    if (bytes_sent == -1)
    {
        std::cerr << "Failed to send data to server\n";
        return 1;
    }

    // Wait for response from server
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
    {
        std::cerr << "Failed to receive data from server\n";
        return 1;
    }

    std::string response(buffer, bytes_received);
    std::cout << response << std::endl;
    if (response == "Serial number already exists.")
    {
        std::cout << "Please enter a different serial number." << std::endl;
        readSerialNumber(client_socket);
    }
    return 0;
}

int readRegistrationNumber(int client_socket)
{
    std::string registration_number;
    std::cout << "Enter a Registration Number (P15/...): ";
    std::getline(std::cin, registration_number);

    // Send the registration number to the server
    int bytes_sent = send(client_socket, registration_number.c_str(), registration_number.size(), 0);
    if (bytes_sent == -1)
    {
        std::cerr << "Failed to send data to server\n";
        return 1;
    }

    // Wait for response from server
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
    {
        std::cerr << "Failed to receive data from server\n";
        return 1;
    }

    std::string response(buffer, bytes_received);
    std::cout << response << std::endl;
    if (response == "Registration number already exists")
    {
        std::cout << "Please enter a different registration number." << std::endl;
        readRegistrationNumber(client_socket);
    }
    return 0;
}

// Server path:
// cd "/Users/ezra/Desktop/Development/School_Codes/Networking/Question 1/4Concurrent-Connection-Oriented/" && g++ server.cpp -o server && "/Users/ezra/Desktop/Development/School_Codes/Networking/Question 1/4Concurrent-Connection-Oriented/"server