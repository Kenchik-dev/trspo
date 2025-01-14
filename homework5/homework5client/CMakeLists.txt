cmake_minimum_required(VERSION 3.29)
project(homework5client)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework5client main.cpp)
target_link_libraries(homework5client PRIVATE ws2_32)
