#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Serial Number Program
int main() {
    std::string filename = "serial_reg_and_name.txt";
    std::string serial_number;
    std::string registration_number;
    std::string name;

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

        // Handle multiple requests from this client
        while (true) {
            // Open IO streams for this connection read-write
            char buffer[1024];
            int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                std::cerr << "Failed to receive data from client\n";
                break;
            }

            // Check if the client wants to exit
            if (std::string(buffer, bytes_received) == "exit") {
                break;
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
                continue;
            }

            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                std::cerr << "Failed to receive data from client\n";
                break;
            }

            registration_number = std::string(buffer, bytes_received);

            exists = false;
            infile.open(filename);
            while (getline(infile, line)) {
                if (line == registration_number) {
                    exists = true;
                    break;
                }
            }
            infile.close();

            if (!exists) {
                std::ofstream outfile(filename, std::ios_base::app);
                outfile << registration_number << "\n";
                outfile.close();
                send(client_socket, "Registration number added.\n", 25, 0);
            } else {
                send(client_socket, "Registration number already exists.\n", 34, 0);
            }

            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                std::cerr << "Failed to receive data from client\n";
                break;
            }

            name = std::string(buffer, bytes_received);

            std::ofstream outfile(filename, std::ios_base::app);
            outfile << name << "\n";
            outfile.close();
            send(client_socket, "Name added.\n", 11, 0);
        }

        std::cout << "Closing connection with client\n";
        close(client_socket);
    }

    close(server_socket);

    return 0;
}