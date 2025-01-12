cmake_minimum_required(VERSION 3.29)
project(homework6server)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework6server main.cpp)
target_link_libraries(homework6server PRIVATE ws2_32)
