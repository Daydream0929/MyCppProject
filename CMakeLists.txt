cmake_minimum_required(VERSION 3.22)
project(MyCppProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyCppProject src/main.cpp src/server/server.h src/server/server.cpp src/client/client.cpp src/client/client.h)
