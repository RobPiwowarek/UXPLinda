#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/tuple/TupleSpace.h"
#include "../../src/server/Request.h"

char* outputTest(std::string tuple) {
    try {
        Tuple temp = Tuple(tuple);
    }catch(const std::invalid_argument &error){
        std::cout<<error.what();
        return nullptr;
    }
    int tupleSize=tuple.size();
    Request::RequestType type=Request::RequestType::OUTPUT;
    int8_t byteType=0;
    const char * tupleChar= tuple.c_str();
    int msgSize=4+4+4+tupleSize+1;
    int pid=1234;
    char* str = new char[msgSize];
    memcpy(str, &type, 4);
    memcpy(str+4, &pid, 4);
    memcpy(str+8, &tupleSize, 4);
    memcpy(str+12, tupleChar, tupleSize+1);
    return str;
}

BOOST_AUTO_TEST_SUITE (TupleSpaceTests)

    BOOST_AUTO_TEST_CASE (get_pid){
        char* str=outputTest("(4)");
        int pid;
        memcpy(&pid, str+4, 4);
        BOOST_CHECK_EQUAL(pid, 1234);
    }
    BOOST_AUTO_TEST_CASE (get_type){
        char* str=outputTest("(4)");
        Request::RequestType type;
        memcpy(&type, str, 4);
        BOOST_CHECK( type == Request::RequestType::OUTPUT);
    }
    BOOST_AUTO_TEST_CASE (get_datasize){
        char* str=outputTest("(4)");
        int size;
        memcpy(&size, str+8, 4);
        BOOST_CHECK_EQUAL(size, 3);
    }

    BOOST_AUTO_TEST_CASE (get_data){
    char* str=outputTest("(4)");
    int size;
    memcpy(&size, str+8, 4);
        char * data = new char[size];
        memcpy(data, str+12, size);
        char result[] = "(4)";
    BOOST_CHECK((strcmp(result, data) == 0));
}

BOOST_AUTO_TEST_SUITE_END()


