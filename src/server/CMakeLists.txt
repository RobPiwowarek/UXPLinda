cmake_minimum_required(VERSION 3.8)
project(UXPLinda/server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES CMDParser.h CMDParser.cpp Request.h Request.cpp Server.h Server.cpp)

add_library(server STATIC ${SOURCE_FILES})