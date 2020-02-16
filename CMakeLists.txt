cmake_minimum_required(VERSION 3.14)
project(twilight)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DDEBUG)
add_definitions(-DUNICODE)

include_directories("thirdpart/dx/inc")

link_directories("thirdpart/dx/lib/x86")

add_executable(twilight main.cpp logs/logs_simple.cpp logs/logs_simple.h)