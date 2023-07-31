//
// Created by jucilab on 23-7-27.
//

#include "server.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "../utils/error_handler.h"

Server::Server(int port) : port(port), server_socket(-1) {}

void Server::CreateSocket()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) { ErrorHandler::HandleError("Failed to create server socket."); }
}

void Server::BindSocket()
{
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        ErrorHandler::HandleError("Failed to bind server socket.");
        close(server_socket);
    }
}

void Server::ListenForConnections()
{
    if (listen(server_socket, 5) < 0)
    {
        ErrorHandler::HandleError("Failed to listen for connections.");
        close(server_socket);
    }
    std::cout << "Server started. Waiting for connections..." << std::endl;
}

void Server::AcceptClient()
{
    int epollFd = epoll_create1(0);
    if (epollFd == -1) { ErrorHandler::HandleError("Failed to create epoll instance."); }

    struct epoll_event event;
    event.data.fd = server_socket;
    event.events  = EPOLLIN | EPOLLET;  // Enable edge-triggered mode
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, server_socket, &event) == -1)
    {
        ErrorHandler::HandleError("Failed to add server socket to epoll instance.");
    }

    std::vector<struct epoll_event> events(10);  // You can adjust the size as needed

    while (true)
    {
        int num_events = epoll_wait(epollFd, events.data(), events.size(), 500);
        if (num_events == -1) { ErrorHandler::HandleError("Failed in epoll_wait."); }
        else if (num_events == 0)
        {
            // Timeout, no events
            // Perform other tasks or checks, if needed
            continue;
        }

        for (int i = 0; i < num_events; ++i)
        {
            if (events[i].data.fd == server_socket) { HandleNewConnection(epollFd); }
            else if (events[i].events & (EPOLLIN | EPOLLET))
            {
                HandleClientData(events[i].data.fd);
            }
        }
    }
}

void Server::HandleNewConnection(int epollFd)
{
    while (true)
    {
        socklen_t client_addr_size = sizeof(client_addr);
        int       client_socket =
            accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size);
        if (client_socket == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more pending connections
                break;
            }
            else { ErrorHandler::HandleError("Failed to accept client connection."); }
        }

        std::cout << "Client connected. Socket fd: " << client_socket << std::endl;

        // Set the client socket to non-blocking mode
        int flags = fcntl(client_socket, F_GETFL, 0);
        fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

        struct epoll_event event;
        event.data.fd = client_socket;
        event.events  = EPOLLIN | EPOLLET;  // Enable edge-triggered mode
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client_socket, &event) == -1)
        {
            ErrorHandler::HandleError("Failed to add client socket to epoll.");
        }
    }
}

void Server::HandleClientData(int client_socket)
{
    // Set the client socket to non-blocking mode
    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

    char buffer[1024];
    int  bytes_read = 0;
    while (true)
    {
        // 在非阻塞模式下，使用 recv() 时需要考虑 EAGAIN 或 EWOULDBLOCK 错误
        bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_read == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 没有数据可读，等待下一个事件通知
                break;
            }
            else
            {
                ErrorHandler::HandleError("Error: Failed to receive data from client.");
                close(client_socket);
                return;
            }
        }
        else if (bytes_read == 0)
        {
            // 客户端关闭了连接
            std::cout << "Client closed the connection." << std::endl;
            close(client_socket);
            return;
        }

        // 在这里处理接收到的数据
        // 例如，可以将数据发送回客户端
        buffer[bytes_read] = '\0';
        send(client_socket, buffer, bytes_read, 0);
    }
}

void Server::Start()
{
    CreateSocket();
    BindSocket();
    ListenForConnections();
    AcceptClient();
}

Server::~Server()
{
    // Clean up resources
    close(server_socket);
}