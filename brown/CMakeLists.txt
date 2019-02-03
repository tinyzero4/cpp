cmake_minimum_required(VERSION 3.12)
project(brown C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wfatal-errors -pthread -std=c++17 -static-libstdc++")

find_package(Threads)

add_executable(red w3/polynomial.cpp)

target_link_libraries(red ${CMAKE_THREAD_LIBS_INIT})