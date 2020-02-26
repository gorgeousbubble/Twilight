cmake_minimum_required(VERSION 3.14)
project(twilight)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DDEBUG)
add_definitions(-DUNICODE)
add_definitions(-DTWILIGHT_EXPORTS)

include_directories("thirdpart/dx/inc")

link_directories("thirdpart/dx/lib/x86")

add_executable(twilight main.cpp logs/logs_simple.cpp logs/logs_simple.h utils/utils_thread_lock.cpp utils/utils_thread_lock.h utils/utils_thread_lock_ex.cpp utils/utils_thread_lock_ex.h nets/nets_serial.cpp nets/nets_serial.h)