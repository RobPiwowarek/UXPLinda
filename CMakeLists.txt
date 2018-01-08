cmake_minimum_required(VERSION 3.8)
project(UXPLinda)
#find_package (Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(src/tuple)

add_executable(UXPLindaDBG main.cpp)

target_link_libraries(UXPLindaDBG
        client server tuple
        ${CMAKE_THREAD_LIBS_INIT})


#add_executable(UXPLinda main.cpp src/server/Server.cpp src/server/Server.h src/client/Client.cpp src/client/Client.h src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h src/server/Request.cpp src/server/Request.h src/server/CMDParser.cpp src/server/CMDParser.h)

#target_link_libraries(UXPLinda ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

#add_test(boostUnitTestsRun UXPLinda)