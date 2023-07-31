//
// Created by jucilab on 23-7-27.
//

#ifndef MYCPPPROJECT_SERVER_H
#define MYCPPPROJECT_SERVER_H

#include <arpa/inet.h>

class Server
{
public:
    explicit Server(int port);
    // 声明 start() 函数为虚函数
    virtual void Start();
    virtual ~Server();

private:
    // Declare private members and helper functions here
    int         port;
    int         server_socket;
    sockaddr_in server_addr;
    sockaddr_in client_addr;

    void CreateSocket();
    void BindSocket();
    void ListenForConnections();
    void AcceptClient();
    void HandleNewConnection(int epollFd);
    void HandleClientData(int epollFd);
};

#endif  // MYCPPPROJECT_SERVER_H
