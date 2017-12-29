cmake_minimum_required(VERSION 3.9)
project(UXPLinda)

set(CMAKE_CXX_STANDARD 11)

add_executable(UXPLinda main.cpp src/server/Server.cpp src/server/Server.h src/client/Client.cpp src/client/Client.h src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h src/server/Request.cpp src/server/Request.h)