cmake_minimum_required(VERSION 3.29)
project(homework4server)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework4server main.cpp)
target_link_libraries(homework4server PRIVATE ws2_32)