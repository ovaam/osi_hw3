cmake_minimum_required(VERSION 3.28)
project(some)

set(CMAKE_CXX_STANDARD 17)

add_executable(some
        server.cpp
        common.h
        worker1_5.cpp
        worker2_5.cpp
        worker3_5.cpp
        monitor_client.cpp
        client.cpp)
