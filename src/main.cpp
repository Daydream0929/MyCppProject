#include <iostream>
#include "client/client.h"
#include "server/server.h"
#include <thread>
#include <vector>

int main() {
    // 创建服务器线程
    std::thread serverThread([]() {
        Server server(8080);
        server.start();
    });

    // 等待一会儿，以确保服务器线程已经启动并开始监听
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 创建多个客户端线程
    const int numClients = 5; // 设置要创建的客户端数量
    std::vector<std::thread> clientThreads;
    for (int i = 0; i < numClients; ++i) {
        clientThreads.emplace_back([]() {
            Client client("127.0.0.1", 8080);
            client.connectToServer();
            client.communicate();
        });
    }

    // 等待所有客户端线程和服务器线程结束
    for (auto& thread : clientThreads) {
        thread.join();
    }
    serverThread.join();

    return 0;
}
