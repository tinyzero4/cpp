cmake_minimum_required(VERSION 3.12)
project(app)

set (app_VERSION_MAJOR 1)
set (app_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -std=c++14 -stdlib=libc++")

add_executable(app w6/test_runner.cpp w6/main.cpp w6/date.cpp w6/database.cpp w6/token.cpp w6/node.cpp w6/condition_parser.cpp w6/condition_parser_test.cpp w6/database_test.cpp w6/node_test.cpp w6/date_test.cpp)
