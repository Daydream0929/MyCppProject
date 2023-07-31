cmake_minimum_required(VERSION 3.22)
project(MyCppProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyCppProject src/main.cpp src/server/server.h src/server/server.cpp src/client/client.cpp src/client/client.h src/utils/error_handler.cpp src/utils/error_handler.h src/server/server_epoll.cpp src/server/server_epoll.h src/server/server_select.cpp src/server/server_select.h src/server/server_poll.cpp src/server/server_poll.h)
