#include <iostream>
#include <thread>
#include <vector>

#include "client/client.h"
#include "server/server.h"
#include "server/server_epoll.h"

void testServerWithClients()
{
    // 创建服务器对象，并使用 epoll 模式启动服务器
    ServerEpoll server(8080);

    server.Start();
}

void testClient()
{
    // 创建客户端对象，并连接到服务器，然后与服务器通信
    Client client("127.0.0.1", 8080);
    client.connectToServer();
    client.communicate();
}

int main()
{
    // 创建服务器线程
    std::thread serverThread(testServerWithClients);

    // 等待一会儿，以确保服务器线程已经启动并开始监听
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 创建多个客户端线程
    const int                numClients = 1;  // 设置要创建的客户端数量
    std::vector<std::thread> clientThreads;
    clientThreads.reserve(numClients);
    for (int i = 0; i < numClients; ++i) { clientThreads.emplace_back(testClient); }

    // 等待所有客户端线程和服务器线程结束
    for (auto& thread : clientThreads) { thread.join(); }
    serverThread.join();

    return 0;
}
