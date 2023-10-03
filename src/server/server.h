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

    void CreateSocket();

    void BindSocket();

    void ListenForConnections();

    // 声明 虚函数
    virtual void Start()        ;
    virtual void AcceptClient() ;

    virtual void HandleNewConnection(int epollFd);

    virtual void HandleClientData(int epollFd);
    virtual ~Server();

protected:
    int         port;
    int         server_socket;
    sockaddr_in client_addr;
    sockaddr_in server_addr;
};

#endif  // MYCPPPROJECT_SERVER_H
