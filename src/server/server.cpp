//
// Created by jucilab on 23-7-27.
//

#include "server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Server::Server(int port) : port(port), serverSocket(-1), clientSocket(-1) {}

void Server::createSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
        exit(1);
    }
}

void Server::bindSocket() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error binding socket" << std::endl;
        close(serverSocket);
        exit(1);
    }
}

void Server::listenForConnections() {
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Error: Failed to listen for connections." << std::endl;
        close(serverSocket);
        exit(1);
    }
    std::cout << "Server started. Waiting for connections..." << std::endl;
}

void Server::acceptClient() {
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrSize);
    if (clientSocket < 0) {
        std::cerr << "Error: Failed to accept client connection." << std::endl;
        close(serverSocket);
        exit(1);
    }
    std::cout << "Client connected." << std::endl;
}

void Server::communicate() {
    char buffer[1024];
    int bytesReceived = 0;
    while (true) {
        // Receive message from client
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived < 0) {
            std::cerr << "Error: Failed to receive message from client." << std::endl;
            close(serverSocket);
            close(clientSocket);
            exit(1);
        }
        if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        std::cout << "Client: " << buffer << std::endl;

        // Send message to client
        std::cout << "Server: ";
        std::cin.getline(buffer, 1024);
        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            std::cerr << "Error: Failed to send message to client." << std::endl;
            close(serverSocket);
            close(clientSocket);
            exit(1);
        }
    }
}

void Server::start() {
    createSocket();
    bindSocket();
    listenForConnections();
    acceptClient();
    communicate();
}

Server::~Server() {
    // Clean up resources
    close(clientSocket);
    close(serverSocket);
}