//
// Created by jucilab on 23-7-31.
//

#ifndef MYCPPPROJECT_SERVER_EPOLL_H
#define MYCPPPROJECT_SERVER_EPOLL_H

#include "server.h"

class ServerEpoll : public Server
{
public:
    explicit ServerEpoll(int port);
    ~ServerEpoll() override;

    // 实现基类的虚函数
    void AcceptClient() override;

    void HandleNewConnection(int epollFd) override;

    void HandleClientData(int epollFd) override;
    void Start() override;
};

#endif  // MYCPPPROJECT_SERVER_EPOLL_H
