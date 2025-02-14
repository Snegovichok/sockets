cmake_minimum_required(VERSION 3.10)
project(Client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(client client.cpp)
target_include_directories(client PRIVATE ../lib)
target_link_libraries(client PRIVATE mylib pthread)

