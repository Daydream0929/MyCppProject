//
// Created by jucilab on 23-7-31.
//

#include "server_epoll.h"

ServerEpoll::ServerEpoll(int port) : Server(port) {
    // 在这里初始化 epoll 相关的参数
}

ServerEpoll::~ServerEpoll() {
    // 在这里清理 epoll 相关的资源
}

void ServerEpoll::start() {
    // 在这里实现 epoll 模式的服务器逻辑

}
