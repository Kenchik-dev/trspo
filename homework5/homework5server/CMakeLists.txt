cmake_minimum_required(VERSION 3.29)
project(homework5server)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework5server main.cpp)
target_link_libraries(homework5server PRIVATE ws2_32)
