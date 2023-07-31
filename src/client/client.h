//
// Created by jucilab on 23-7-27.
//

#ifndef MYCPPPROJECT_CLIENT_H
#define MYCPPPROJECT_CLIENT_H

#include <arpa/inet.h>

#include <string>

class Client
{
public:
    Client(const std::string& server_address, int port);
    void connectToServer();
    void communicate();
    ~Client();

private:
    // Declare private members and helper functions here
    std::string serverAddress;
    int         port;
    int         client_socket;
    sockaddr_in server_addr;

    void CreateSocket();
    void ConfigureServerAddress();
    void ConnectToServerSocket();
};

#endif  // MYCPPPROJECT_CLIENT_H
