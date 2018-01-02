cmake_minimum_required(VERSION 3.8)
project(UXPLinda/tuple)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Pattern.cpp Pattern.h Tuple.cpp Tuple.h TupleSpace.h TupleSpace.cpp)

add_library(tuple STATIC ${SOURCE_FILES})