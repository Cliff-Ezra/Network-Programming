#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int main() {
    // Step 1: Include necessary libraries
    std::string url = "www.example.com"; // URL of the desired web page
    std::string request = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n"; // HTTP GET request string

    // Step 3: Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    // Step 4: Establish a connection to the web server
    struct hostent *host = gethostbyname(url.c_str());
    if (!host) {
        std::cerr << "Failed to resolve hostname\n";
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = *((unsigned long *) host->h_addr);
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        return -1;
    }

    // Step 5: Send an HTTP GET request
    if (send(sock, request.c_str(), request.length(), 0) < 0) {
        std::cerr << "Failed to send request\n";
        return -1;
    }

    // Step 6: Receive the server's response
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    std::string response;
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        response.append(buffer, bytes_received);
    }
    if (bytes_received < 0) {
        std::cerr << "Failed to receive response\n";
        return -1;
    }

    // Step 7: Process the received data
    std::cout << response << "\n";

    // Step 8: Close the socket
    close(sock);

    return 0;
}