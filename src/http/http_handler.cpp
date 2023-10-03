//
// Created by jucilab on 23-8-1.
//

#include "http_handler.h"

#include <iostream>

std::string HttpHandler::handleRequest(const std::string& request) {
    // Implement the HTTP request handling logic here
    // This is a simplified version, you can parse the request, handle various HTTP methods, headers, and request bodies.

    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "\r\n"
                           "<html><body><h1>Hello, World!</h1></body></html>";
    return response;
}
