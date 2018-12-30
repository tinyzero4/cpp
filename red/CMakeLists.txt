cmake_minimum_required(VERSION 3.12)
project(red)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wfatal-errors")

add_executable(red w5/priority_collection.cpp)