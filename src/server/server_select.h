//
// Created by jucilab on 23-7-31.
//

#ifndef MYCPPPROJECT_SERVER_SELECT_H
#define MYCPPPROJECT_SERVER_SELECT_H

#include "server.h"

class ServerSelect : public Server
{
public:
    explicit ServerSelect(int port);
    virtual ~ServerSelect();

    // 实现基类的虚函数
    virtual void Start() override;
};

#endif  // MYCPPPROJECT_SERVER_SELECT_H
