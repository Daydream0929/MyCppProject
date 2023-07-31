#include "client.h"
#include "../utils/error_handler.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

Client::Client(const std::string& serverAddress, int port)
        : serverAddress(serverAddress), port(port), clientSocket(-1) {}

void Client::createSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        ErrorHandler::handleError("Error: Failed to create socket.");
    }

    // Set the client socket to non-blocking mode
    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
}

void Client::configureServerAddress() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress.c_str(), &(serverAddr.sin_addr));
}

void Client::connectToServerSocket() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        if (errno != EINPROGRESS) {
            ErrorHandler::handleError("Error: Failed to connect to server.");
            close(clientSocket);
        }
    }

    std::cout << "Connecting to server..." << std::endl;

    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(clientSocket, &writeSet);

    struct timeval timeout;
    timeout.tv_sec = 5; // Set a 5-second timeout (adjust as needed)
    timeout.tv_usec = 0;

    int selectResult = select(clientSocket + 1, NULL, &writeSet, NULL, &timeout);
    if (selectResult == -1) {
        ErrorHandler::handleError("Error: select() failed.");
        close(clientSocket);
    } else if (selectResult == 0) {
        ErrorHandler::handleError("Error: Connection timed out.");
        close(clientSocket);
    }

    int socketError = 0;
    socklen_t socketErrorSize = sizeof(socketError);
    getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &socketError, &socketErrorSize);
    if (socketError != 0) {
        ErrorHandler::handleError("Error: Failed to connect to server.");
        close(clientSocket);
    }

    std::cout << "Connected to server." << std::endl;
}

void Client::communicate() {
    char buffer[1024];
    while (true) {
        std::cout << "Enter message (or 'q' to quit): ";
        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "q") == 0) {
            break;
        }

        // Send data in non-blocking mode
        int totalBytesSent = 0;
        int messageLength = strlen(buffer);
        while (totalBytesSent < messageLength) {
            int bytesSent = send(clientSocket, buffer + totalBytesSent, messageLength - totalBytesSent, 0);
            if (bytesSent == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Send buffer is full, try again later
                    continue;
                } else {
                    ErrorHandler::handleError("Error: Failed to send data to the server.");
                    break;
                }
            }
            totalBytesSent += bytesSent;
        }

        // Receive data in non-blocking mode
        int totalBytesReceived = 0;
        while (totalBytesReceived < sizeof(buffer) - 1) {
            int bytesRead = recv(clientSocket, buffer + totalBytesReceived, sizeof(buffer) - 1 - totalBytesReceived, 0);
            if (bytesRead == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // No data available to read, try again later
                    break;
                } else {
                    ErrorHandler::handleError("Error: Failed to receive data from the server.");
                    break;
                }
            } else if (bytesRead == 0) {
                // Server closed the connection
                std::cout << "Server closed the connection." << std::endl;
                break;
            }
            totalBytesReceived += bytesRead;
        }

        buffer[totalBytesReceived] = '\0';
        std::cout << "Server replied: " << buffer << std::endl;
    }
}

void Client::connectToServer() {
    createSocket();
    configureServerAddress();
    connectToServerSocket();
}

Client::~Client() {
    close(clientSocket);
}
