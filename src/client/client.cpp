//
// Created by jucilab on 23-7-27.
//

#include "client.h"
#include "../utils/error_handler.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

Client::Client(const std::string& serverAddress, int port)
        : serverAddress(serverAddress), port(port), clientSocket(-1) {}

void Client::createSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        ErrorHandler::handleError("Error: Failed to create socket.");
    }
}

void Client::configureServerAddress() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress.c_str(), &(serverAddr.sin_addr));
}

void Client::connectToServerSocket() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
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

        send(clientSocket, buffer, strlen(buffer), 0);

        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }

        buffer[bytesRead] = '\0';
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