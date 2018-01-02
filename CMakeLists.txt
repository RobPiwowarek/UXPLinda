cmake_minimum_required(VERSION 3.5)
project(UXPLinda)
find_package (Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(UXPLinda tests/main_tests.cpp main.cpp src/server/Server.cpp src/server/Server.h src/client/Client.cpp src/client/Client.h src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h src/server/Request.cpp src/server/Request.h src/server/CMDParser.cpp src/server/CMDParser.h)
target_link_libraries(UXPLinda ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun UXPLinda)