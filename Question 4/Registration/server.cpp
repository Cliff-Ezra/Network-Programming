#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Define the message header
struct MessageHeader
{
    uint32_t type;
    uint32_t length;
};

// Define the message struct
struct Message
{
    MessageHeader header;
    char body[1024];
};

// Process the request and generate a response
void processRequest(char *request, char *response, size_t responseSize)
{
    // console log the request
    std::cout << "Received request: " << request << std::endl;
    // copy the request into the response
    char *token;
    token = strtok(request, " ");
    int x = atoi(token);
    token = strtok(NULL, " ");
    int y = atoi(token);
    token = strtok(NULL, " ");
    int op = atoi(token);
    switch (op)
    {
    case 1:
        std::sprintf(response, "%d + %d = %d", x, y, x + y);
        break;
    case 2:
        std::sprintf(response, "%d - %d = %d", x, y, x - y);
        break;
    case 3:
        std::sprintf(response, "%d * %d = %d", x, y, x * y);
        break;
    case 4:
        if (y == 0)
        {
            std::sprintf(response, "Error: division by zero");
        }
        else
        {
            std::sprintf(response, "%d / %d = %d", x, y, x / y);
        }
        break;
    default:
        std::sprintf(response, "Error: invalid operation");
        break;
    }
}
// Serialize a message struct into a character buffer
void serializeMessage(const Message &message, char *buffer, size_t bufferSize)
{
    std::memcpy(buffer, &message.header, sizeof(MessageHeader));
    std::memcpy(buffer + sizeof(MessageHeader), message.body, message.header.length);
}

// Deserialize a character buffer into a message struct
void deserializeMessage(const char *buffer, size_t bufferSize, Message &message)
{
    std::memcpy(&message.header, buffer, sizeof(MessageHeader));
    std::memcpy(message.body, buffer + sizeof(MessageHeader), message.header.length);
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Error: Failed to create server socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error: Failed to bind server socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        std::cerr << "Error: Failed to listen for incoming connections." << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "Server is listening for incoming connections." << std::endl;

    while (true)
    {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == -1)
        {
            std::cerr << "Error: Failed to accept client connection." << std::endl;
            continue;
        }

        std::cout << "Client connected from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

        // Receive the request from the client
        Message requestMessage{};
        recv(clientSocket, &requestMessage.header, sizeof(MessageHeader), MSG_WAITALL);
        recv(clientSocket, requestMessage.body, requestMessage.header.length, MSG_WAITALL);

        // Process the request and generate a response
        char response[1024];
        processRequest(requestMessage.body, response, sizeof(response));

        // Send the response to the client
        Message responseMessage{};
        responseMessage.header.type = 2;                           // Response message
        responseMessage.header.length = std::strlen(response) + 1; // Include the null terminator
        std::strcpy(responseMessage.body, response);

        char responseBuffer[sizeof(MessageHeader) + sizeof(response)];
        serializeMessage(responseMessage, responseBuffer, sizeof(responseBuffer));

        send(clientSocket, responseBuffer, sizeof(responseBuffer), 0);

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
