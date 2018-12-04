cmake_minimum_required(VERSION 3.12)
#set(CMAKE_CXX_COMPILER "/usr/local/bin/gcc-8")
project(red)

set(CMAKE_CXX_STANDARD 14)

add_executable(red w1/paginator.cpp)