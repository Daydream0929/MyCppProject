//
// Created by jucilab on 23-7-31.
//

#ifndef MYCPPPROJECT_SERVER_POLL_H
#define MYCPPPROJECT_SERVER_POLL_H

#include "server.h"

class ServerPoll : public Server {
public:
    ServerPoll(int port);
    virtual ~ServerPoll();

    // 实现基类的虚函数
    virtual void start() override;
};



#endif //MYCPPPROJECT_SERVER_POLL_H
