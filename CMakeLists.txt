cmake_minimum_required(VERSION 3.8)
project(UXPLinda)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(src)

add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(src/tuple)

add_executable(UXPLindaDBG main.cpp)

target_link_libraries(UXPLindaDBG
        client server tuple
        ${CMAKE_THREAD_LIBS_INIT})

#cmake_minimum_required(VERSION 3.5)
#project(UXPLinda)
#find_package (Boost COMPONENTS unit_test_framework REQUIRED)
#
#add_executable(UXPLinda tests/main_tests.cpp src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h tests/tuple/PatternTests.cpp)
#target_link_libraries(UXPLinda ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#
#add_test(boostUnitTestsRun UXPLinda)