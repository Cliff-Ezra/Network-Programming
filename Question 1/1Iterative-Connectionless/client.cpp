#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int readSerailNumber(int client_socket, sockaddr_in server_address, socklen_t server_address_size);
int readRegistrationNumber(int client_socket, sockaddr_in server_address, socklen_t server_address_size);

// Serial Number Program
int main()
{
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "Failed to create client socket\n";
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::string serial_number;
    std::string registration_number;
    std::string name;

    // Loop until the user chooses to exit
    while (true)
    {
        // Read the serial number from the user
        int serial_number = readSerailNumber(client_socket, server_address, sizeof(server_address));
        // check if there was an error
        if (serial_number == 1)
        {
            std::cerr << "Failed to read serial number\n";
            break;
        }
        // Read the registration number from the user
        int registration_number = readRegistrationNumber(client_socket, server_address, sizeof(server_address));
        // check if there was an error
        if (registration_number == 1)
        {
            std::cerr << "Failed to read registration number\n";
            break;
        }
        int bytes_sent;
        int bytes_received;
        char buffer[1024];
        std::string response;
        socklen_t server_address_size = sizeof(server_address);

        std::cout << "Enter a name: ";
        std::getline(std::cin, name);

        // Send the name to the server
        bytes_sent = sendto(client_socket, name.c_str(), name.size(), 0, (sockaddr *)&server_address, sizeof(server_address));
        if (bytes_sent == -1)
        {
            std::cerr << "Failed to send data to server\n";
            continue;
        }

        // Wait for response from server
        bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (sockaddr *)&server_address, &server_address_size);
        if (bytes_received == -1)
        {
            std::cerr << "Failed to receive data from server\n";
            continue;
        }
        response = std::string(buffer, bytes_received);
        std::cout << response << std::endl;
    }

    // Close the socket
    close(client_socket);

    return 0;
}

int readSerailNumber(int client_socket, sockaddr_in server_address, socklen_t server_address_size)
{
    std::string serial_number;
    std::cout << "Enter a serial number (or 'exit' to quit): ";
    std::getline(std::cin, serial_number);

    // Check if the user wants to exit
    if (serial_number == "exit")
    {
        sendto(client_socket, serial_number.c_str(), serial_number.size(), 0, (sockaddr *)&server_address, sizeof(server_address));
        return 0;
    }

    // Send the serial number to the server
    int bytes_sent = sendto(client_socket, serial_number.c_str(), serial_number.size(), 0, (sockaddr *)&server_address, sizeof(server_address));
    if (bytes_sent == -1)
    {
        std::cerr << "Failed to send data to server\n";
        return 1;
    }

    // Wait for response from server
    char buffer[1024];
    int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (sockaddr *)&server_address, &server_address_size);
    if (bytes_received == -1)
    {
        std::cerr << "Failed to receive data from server\n";
        return 1;
    }
    std::string response(buffer, bytes_received);
    std::cout << response << std::endl;
    if (response == "Serial number already exists.")
    {
        // prompt for serial number again
        std::cout << "Please enter a different serial number." << std::endl;
        readSerailNumber(client_socket, server_address, server_address_size);
    }
    return 0;
}

int readRegistrationNumber(int client_socket, sockaddr_in server_address, socklen_t server_address_size)
{
    std::string registration_number;
    std::cout << "Enter a registration number (or 'exit' to quit): ";
    std::getline(std::cin, registration_number);

    // Check if the user wants to exit
    if (registration_number == "exit")
    {
        sendto(client_socket, registration_number.c_str(), registration_number.size(), 0, (sockaddr *)&server_address, sizeof(server_address));
        return 0;
    }

    // Send the registration number to the server
    int bytes_sent = sendto(client_socket, registration_number.c_str(), registration_number.size(), 0, (sockaddr *)&server_address, sizeof(server_address));
    if (bytes_sent == -1)
    {
        std::cerr << "Failed to send data to server\n";
        return 1;
    }

    // Wait for response from server
    char buffer[1024];
    int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (sockaddr *)&server_address, &server_address_size);
    if (bytes_received == -1)
    {
        std::cerr << "Failed to receive data from server\n";
        return 1;
    }
    std::string response(buffer, bytes_received);
    std::cout << response << std::endl;
    if (response == "Registration number already exists")
    {
        // prompt for registration number again
        std::cout << "Please enter a different registration number." << std::endl;
        readRegistrationNumber(client_socket, server_address, server_address_size);
    }
    return 0;
}
// Server Path
// "/Users/ezra/Desktop/Development/School_Codes/Networking/Question 1/1Iterative-Connectionless/" && g++ server.cpp -o server && "/Users/ezra/Desktop/Development/School_Codes/Networking/Question 1/1Iterative-Connectionless/"server