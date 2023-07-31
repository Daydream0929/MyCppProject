#include "client.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "../utils/error_handler.h"

Client::Client(const std::string& server_address, int port)
    : serverAddress(server_address), port(port), client_socket(-1)
{
}

void Client::CreateSocket()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) { ErrorHandler::handleError("Error: Failed to create socket."); }

    // Set the client socket to non-blocking mode
    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
}

void Client::ConfigureServerAddress()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    inet_pton(AF_INET, serverAddress.c_str(), &(server_addr.sin_addr));
}

void Client::ConnectToServerSocket()
{
    if (connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
        if (errno != EINPROGRESS)
        {
            ErrorHandler::handleError("Error: Failed to connect to server.");
            close(client_socket);
        }
    }

    std::cout << "Connecting to server..." << std::endl;

    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(client_socket, &write_set);

    struct timeval timeout;
    timeout.tv_sec  = 5;  // Set a 5-second timeout (adjust as needed)
    timeout.tv_usec = 0;

    int select_result = select(client_socket + 1, NULL, &write_set, NULL, &timeout);
    if (select_result == -1)
    {
        ErrorHandler::handleError("Error: select() failed.");
        close(client_socket);
    }
    else if (select_result == 0)
    {
        ErrorHandler::handleError("Error: Connection timed out.");
        close(client_socket);
    }

    int       socket_error    = 0;
    socklen_t socketErrorSize = sizeof(socket_error);
    getsockopt(client_socket, SOL_SOCKET, SO_ERROR, &socket_error, &socketErrorSize);
    if (socket_error != 0)
    {
        ErrorHandler::handleError("Error: Failed to connect to server.");
        close(client_socket);
    }

    std::cout << "Connected to server." << std::endl;
}

void Client::communicate()
{
    char buffer[1024];
    while (true)
    {
        std::cout << "Enter message (or 'q' to quit): ";
        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "q") == 0) { break; }

        // Send data in non-blocking mode
        int total_bytes_sent = 0;
        int message_length   = strlen(buffer);
        while (total_bytes_sent < message_length)
        {
            int bytes_sent = send(client_socket, buffer + total_bytes_sent,
                                  message_length - total_bytes_sent, 0);
            if (bytes_sent == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // Send buffer is full, try again later
                    continue;
                }
                else
                {
                    ErrorHandler::handleError("Error: Failed to send data to the server.");
                    break;
                }
            }
            total_bytes_sent += bytes_sent;
        }

        // Receive data in non-blocking mode
        int total_bytes_received = 0;
        while (total_bytes_received < sizeof(buffer) - 1)
        {
            int bytes_read = recv(client_socket, buffer + total_bytes_received,
                                  sizeof(buffer) - 1 - total_bytes_received, 0);
            if (bytes_read == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // No data available to read, try again later
                    break;
                }
                else
                {
                    ErrorHandler::handleError("Error: Failed to receive data from the server.");
                    break;
                }
            }
            else if (bytes_read == 0)
            {
                // Server closed the connection
                std::cout << "Server closed the connection." << std::endl;
                break;
            }
            total_bytes_received += bytes_read;
        }

        buffer[total_bytes_received] = '\0';
        std::cout << "Server replied: " << buffer << std::endl;
    }
}

void Client::connectToServer()
{
    CreateSocket();
    ConfigureServerAddress();
    ConnectToServerSocket();
}

Client::~Client() { close(client_socket); }
