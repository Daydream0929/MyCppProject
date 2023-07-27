//
// Created by jucilab on 23-7-27.
//

#ifndef MYCPPPROJECT_CLIENT_H
#define MYCPPPROJECT_CLIENT_H

#include <string>
#include <arpa/inet.h>

class Client {
public:
    Client(const std::string& serverAddress, int port);
    void connectToServer();
    void communicate();
    ~Client();

private:
    // Declare private members and helper functions here
    std::string serverAddress;
    int port;
    int clientSocket;
    sockaddr_in serverAddr;

    void createSocket();
    void configureServerAddress();
    void connectToServerSocket();
};

#endif //MYCPPPROJECT_CLIENT_H
