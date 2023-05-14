#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

struct MessageHeader
{
    int type;   // 1 = Request, 2 = Response
    int length; // Length of message body
};

struct Message
{
    MessageHeader header;
    char body[1024];
};

void serializeMessage(const Message &message, char *buffer, size_t bufferSize)
{
    if (bufferSize < sizeof(message.header) + message.header.length)
    {
        std::cerr << "Error: Message buffer is too small." << std::endl;
        return;
    }

    std::memcpy(buffer, &message.header, sizeof(message.header));
    std::memcpy(buffer + sizeof(message.header), message.body, message.header.length);
}

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Error: Failed to create client socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error: Failed to connect to server." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Prompt the user for two numbers
    int num1, num2;
    std::cout << "Enter num 1: ";
    std::cin >> num1;
    std::cout << "Enter num 2: ";
    std::cin >> num2;

    // Display the menu of operations and prompt the user for a selection
    std::cout << "Select an operation:" << std::endl;
    std::cout << "1. Addition (+)" << std::endl;
    std::cout << "2. Subtraction (-)" << std::endl;
    std::cout << "3. Multiplication (*)" << std::endl;
    std::cout << "4. Division (/)" << std::endl;

    int selection;
    std::cin >> selection;

    // Create the request message
    Message requestMessage{};
    requestMessage.header.type = 1; // Request message

    // Serialize the user input and operation selection into the message body
    std::string requestBody = std::to_string(num1) + " " + std::to_string(num2) + " " + std::to_string(selection);
    requestMessage.header.length = requestBody.length() + 1; // Include the null terminator
    std::strcpy(requestMessage.body, requestBody.c_str());

    char requestBuffer[sizeof(MessageHeader) + sizeof(requestMessage.body)];
    serializeMessage(requestMessage, requestBuffer, sizeof(requestBuffer));

    // Send the request to the server
    send(clientSocket, requestBuffer, sizeof(requestBuffer), 0);

    // Receive the response from the server
    char responseBuffer[sizeof(MessageHeader) + 1024];
    recv(clientSocket, responseBuffer, sizeof(responseBuffer), MSG_WAITALL);

    Message responseMessage{};
    std::memcpy(&responseMessage.header, responseBuffer, sizeof(responseMessage.header));
    std::memcpy(responseMessage.body, responseBuffer + sizeof(responseMessage.header), responseMessage.header.length);

    if (responseMessage.header.type == 2)
    { // Response message
        std::cout << "Server response: " << responseMessage.body << std::endl;
    }
    else
    {
        std::cerr << "Error: Received invalid message type from server." << std::endl;
    }

    close(clientSocket);
    return 0;
}
