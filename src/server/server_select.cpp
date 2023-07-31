//
// Created by jucilab on 23-7-31.
//

#include "server_select.h"

ServerSelect::ServerSelect(int port) : Server(port)
{
    // 在这里初始化 select 相关的参数
}

ServerSelect::~ServerSelect()
{
    // 在这里清理 select 相关的资源
}

void ServerSelect::Start()
{
    // 在这里实现 select 模式的服务器逻辑
}
