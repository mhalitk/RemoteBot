cmake_minimum_required(VERSION 2.8)
project(remote-bot-server)
set(CMAKE_BUILD_TYPE Debug)

macro (add_source to source)
    set (${to} ${${to}} ${source})
endmacro()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (${CMAKE_SYSTEM_NAME} MATCHES Linux)

include_directories(include)
include_directories(src)

link_directories("${PROJECT_SOURCE_DIR}/libs")

# Server

add_source(SERVER_SRC src/server.cpp)
add_source(SERVER_SRC src/BotHandler.cpp)
add_source(SERVER_SRC src/BotService.cpp)
add_source(SERVER_SRC src/ClientHandler.cpp)
add_source(SERVER_SRC src/Net/TCPServer.cpp)
add_source(SERVER_SRC src/Net/TCPConnection.cpp)

add_executable(server ${SERVER_SRC})

# Client

add_source(CLIENT_SRC src/client.cpp)

add_executable(client ${CLIENT_SRC})

# Test

add_source(TEST_SRC test/test.cpp)
add_source(TEST_SRC test/EventEmitterTest.cpp)

add_executable(test ${TEST_SRC})
target_link_libraries(test gtest)