cmake_minimum_required(VERSION 2.8)
project(remote-bot-server)

include_directories(include)

add_executable(server src/server.cpp)
