//
// Created by jucilab on 23-7-31.
//

#include "server_poll.h"

ServerPoll::ServerPoll(int port) : Server(port) {
    // 在这里初始化 poll 相关的参数
}

ServerPoll::~ServerPoll() {
    // 在这里清理 poll 相关的资源
}

void ServerPoll::start() {
    // 在这里实现 poll 模式的服务器逻辑
}
