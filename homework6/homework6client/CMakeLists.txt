cmake_minimum_required(VERSION 3.29)
project(homework6client)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework6client main.cpp)
target_link_libraries(homework6client PRIVATE ws2_32)
