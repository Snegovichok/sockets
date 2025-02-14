cmake_minimum_required(VERSION 3.10)
project(Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server server.cpp)
target_include_directories(server PRIVATE ../lib)
target_link_libraries(server PRIVATE mylib)

