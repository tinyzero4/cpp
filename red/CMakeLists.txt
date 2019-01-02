cmake_minimum_required(VERSION 3.12)
project(red C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wfatal-errors -pthread")

find_package( Threads )

add_executable(red w5/course/part1/main.cpp w5/course/part1/parse.cpp w5/course/part1/search_server.cpp)

target_link_libraries(red ${CMAKE_THREAD_LIBS_INIT})