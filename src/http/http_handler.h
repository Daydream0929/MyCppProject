//
// Created by jucilab on 23-8-1.
//

#ifndef MYCPPPROJECT_HTTP_HANDLER_H
#define MYCPPPROJECT_HTTP_HANDLER_H

#include <string>

class HttpHandler {
public:
    static std::string handleRequest(const std::string& request);
    // Other helper functions for parsing and handling HTTP requests can be added here
};

#endif //MYCPPPROJECT_HTTP_HANDLER_H
