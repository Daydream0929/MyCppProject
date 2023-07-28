//
// Created by jucilab on 23-7-29.
//

#include "error_handler.h"
#include <iostream>
#include <cstdlib>

void ErrorHandler::handleError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}