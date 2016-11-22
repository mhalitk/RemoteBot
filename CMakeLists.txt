cmake_minimum_required(VERSION 2.8)
project(remote-bot-server)

include_directories(include)
include_directories(src)

macro (add_source source)
    set (SRCS ${SRCS} ${source})
endmacro()

add_source(src/main.cpp)
add_source(src/Net/TCPServer.cpp)
add_source(src/Net/TCPConnection.cpp)

add_executable(server ${SRCS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)
