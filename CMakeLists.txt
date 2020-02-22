cmake_minimum_required(VERSION 3.14)
project(twilight)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DDEBUG)
add_definitions(-DUNICODE)
add_definitions(-DTWILIGHT_EXPORTS)

include_directories("thirdpart/dx/inc")
include_directories("thirdpart/glog/inc")

link_directories("thirdpart/dx/lib/x86")
link_directories("thirdpart/glog/lib")

add_executable(twilight main.cpp logs/logs_simple.cpp logs/logs_simple.h logs/logs_glog.cpp logs/logs_glog.h)