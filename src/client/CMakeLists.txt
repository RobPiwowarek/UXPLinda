cmake_minimum_required(VERSION 3.8)
project(UXPLinda/client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Client.cpp Client.h)

add_library(client STATIC ${SOURCE_FILES})