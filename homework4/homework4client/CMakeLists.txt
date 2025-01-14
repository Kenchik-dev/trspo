cmake_minimum_required(VERSION 3.29)
project(homework4client)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework4client main.cpp)
target_link_libraries(homework4client PRIVATE ws2_32)