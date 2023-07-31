//
// Created by jucilab on 23-7-31.
//

#ifndef MYCPPPROJECT_SERVER_EPOLL_H
#define MYCPPPROJECT_SERVER_EPOLL_H

#include "server.h"

class ServerEpoll : public Server {
public:
    ServerEpoll(int port);
    virtual ~ServerEpoll();

    // 实现基类的虚函数
    virtual void start() override;
};

#endif //MYCPPPROJECT_SERVER_EPOLL_H
