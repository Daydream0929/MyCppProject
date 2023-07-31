//
// Created by jucilab on 23-7-27.
//

#include "server.h"
#include "../utils/error_handler.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>

Server::Server(int port) : port(port), serverSocket(-1) {}

void Server::createSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        ErrorHandler::handleError("Failed to create server socket.");
    }
}

void Server::bindSocket() {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        ErrorHandler::handleError("Failed to bind server socket.");
        close(serverSocket);
    }
}

void Server::listenForConnections() {
    if (listen(serverSocket, 5) < 0) {
        ErrorHandler::handleError("Failed to listen for connections.");
        close(serverSocket);
    }
    std::cout << "Server started. Waiting for connections..." << std::endl;
}

void Server::acceptClient() {
    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        ErrorHandler::handleError("Failed to create epoll instance.");
    }

    struct epoll_event event;
    event.data.fd = serverSocket;
    event.events = EPOLLIN | EPOLLET; // Enable edge-triggered mode
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        ErrorHandler::handleError("Failed to add server socket to epoll instance.");
    }

    std::vector<struct epoll_event> events(10); // You can adjust the size as needed

    while (true) {
        int numEvents = epoll_wait(epollFd, events.data(), events.size(), 500);
        if (numEvents == -1) {
            ErrorHandler::handleError("Failed in epoll_wait.");
        } else if (numEvents == 0) {
            // Timeout, no events
            // Perform other tasks or checks, if needed
            continue;
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverSocket) {
                handleNewConnection(epollFd);
            } else if(events[i].events & (EPOLLIN | EPOLLET)) {
                handleClientData(events[i].data.fd);
            }
        }
    }
}

void Server::handleNewConnection(int epollFd) {
    while (true) {
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more pending connections
                break;
            } else {
                ErrorHandler::handleError("Failed to accept client connection.");
            }
        }

        std::cout << "Client connected. Socket fd: " << clientSocket << std::endl;

        // Set the client socket to non-blocking mode
        int flags = fcntl(clientSocket, F_GETFL, 0);
        fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

        struct epoll_event event;
        event.data.fd = clientSocket;
        event.events = EPOLLIN | EPOLLET; // Enable edge-triggered mode
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
            ErrorHandler::handleError("Failed to add client socket to epoll.");
        }
    }
}

void Server::handleClientData(int clientSocket) {
    // Set the client socket to non-blocking mode
    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

    char buffer[1024];
    int bytesRead = 0;
    while (true) {
        // 在非阻塞模式下，使用 recv() 时需要考虑 EAGAIN 或 EWOULDBLOCK 错误
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 没有数据可读，等待下一个事件通知
                break;
            } else {
                ErrorHandler::handleError("Error: Failed to receive data from client.");
                close(clientSocket);
                return;
            }
        } else if (bytesRead == 0) {
            // 客户端关闭了连接
            std::cout << "Client closed the connection." << std::endl;
            close(clientSocket);
            return;
        }

        // 在这里处理接收到的数据
        // 例如，可以将数据发送回客户端
        buffer[bytesRead] = '\0';
        send(clientSocket, buffer, bytesRead, 0);
    }
}

void Server::start() {
    createSocket();
    bindSocket();
    listenForConnections();
    acceptClient();
}

Server::~Server() {
    // Clean up resources
    close(serverSocket);
}