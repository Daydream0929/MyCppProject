//
// Created by jucilab on 23-7-27.
//

#ifndef MYCPPPROJECT_SERVER_H
#define MYCPPPROJECT_SERVER_H

#include <arpa/inet.h>

class Server {
public:

    Server(int port);
    void start();
    ~Server();

private:
    // Declare private members and helper functions here
    int port;
    int serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;

    void createSocket();
    void bindSocket();
    void listenForConnections();
    void acceptClient();
    void handleNewConnection(int epollFd);
    void handleClientData(int epollFd);

};

#endif //MYCPPPROJECT_SERVER_H
