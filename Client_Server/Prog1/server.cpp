// How the Program works:
// This is a basic server implementation for a serial number program. 
// The server listens on port 3000 for incoming connections and accepts TCP connections from clients. 
// Once a connection is established, the server reads a serial number from the client 
// and checks if it exists in a file named "serial_numbers.txt". 
// If the serial number is not found, it is added to the file, and the client is notified. 
// If the serial number already exists, the client is notified accordingly. 
// The server then closes the connection and waits for the next client request.

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Serial Number Program

int main() {
    // The file name is hard coded here, but you could ask the user for the file name
    std::string filename = "serial_numbers.txt";
    std::string serial_number;

    // Create a server-socket (listener) and bind it to a port (higher than 1024)
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind server socket\n";
        return 1;
    }

    // Start listening for requests
    if (listen(server_socket, 1) == -1) {
        std::cerr << "Failed to listen on server socket\n";
        return 1;
    }

    std::cout << "Server listening on port 3000\n";

    while (true) {
        // Create another (normal) socket and build a TCP connection when a request arrives
        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
            std::cerr << "Failed to accept client connection\n";
            continue;
        }

        // Open IO streams for this connection read-write
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive data from client\n";
            close(client_socket);
            continue;
        }

        serial_number = std::string(buffer, bytes_received);

        bool exists = false;
        std::ifstream infile(filename);
        std::string line;
        while (getline(infile, line)) {
            if (line == serial_number) {
                exists = true;
                break;
            }
        }
        infile.close();

        if (!exists) {
            std::ofstream outfile(filename, std::ios_base::app);
            outfile << serial_number << "\n";
            outfile.close();
            send(client_socket, "Serial number added.\n", 20, 0);
        } else {
            send(client_socket, "Serial number already exists.\n", 29, 0);
        }

        std::cout << "Closing connection with client\n";

        close(client_socket);
    }

    close(server_socket);

    return 0;
}