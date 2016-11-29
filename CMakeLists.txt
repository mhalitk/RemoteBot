cmake_minimum_required(VERSION 2.8)
project(remote-bot-server)


macro (add_source to source)
    set (${to} ${${to}} ${source})
endmacro()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)

include_directories(include)
include_directories(src)

# Server

add_source(SERVER_SRC src/server.cpp)
add_source(SERVER_SRC src/ClientHandler.cpp)
add_source(SERVER_SRC src/ServiceProviderHandler.cpp)
add_source(SERVER_SRC src/Net/TCPServer.cpp)
add_source(SERVER_SRC src/Net/TCPConnection.cpp)

add_executable(server ${SERVER_SRC})

# Client

add_source(CLIENT_SRC src/client.cpp)

add_executable(client ${CLIENT_SRC})
