#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

struct ClientData {
    int client_socket;
    std::string filename;

    ClientData(int client_socket, std::string filename)
        : client_socket(client_socket), filename(filename) {}
};

void* handle_client(void* arg) {
    ClientData* client_data = (ClientData*)arg;
    int client_socket = client_data->client_socket;
    std::string filename = client_data->filename;

    // Read messages from the client
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive data from client\n";
            break;
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected\n";
            break;
        }

        // Parse the message
        std::istringstream message(buffer);
        int num1, num2;
        char op;
        message >> num1 >> num2 >> op;

        // Perform the calculation
        int result;
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                result = num1 / num2;
                break;
            default:
                std::cerr << "Invalid operator: " << op << '\n';
                continue;
        }

        // Send the result back to the client
        std::ostringstream response;
        response << result << '\n';
        send(client_socket, response.str().c_str(), response.str().length(), 0);
    }

    // Close the socket
    close(client_socket);
    delete client_data;
    return nullptr;
}

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9999);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    // Display the port the server is running on
    std::cout << "Server running on port " << ntohs(server_address.sin_port) << '\n';

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        std::cerr << "Failed to listen on socket\n";
        return 1;
    }

    while (true) {
        // Accept an incoming connection
        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        // Log a message when a client connects
        std::cout << "Client connected\n";

        // Create a thread to handle the client
        pthread_t thread;
        ClientData* client_data = new ClientData(client_socket, "");
        pthread_create(&thread, nullptr, handle_client, client_data);
        pthread_detach(thread);
    }

    // Close the socket
    close(server_socket);

    return 0;
}