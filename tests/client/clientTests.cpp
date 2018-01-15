#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/tuple/TupleSpace.h"
#include "../../src/server/Request.h"

BOOST_AUTO_TEST_SUITE (TupleSpaceTests)

    BOOST_AUTO_TEST_CASE (get_pid) {
        Client client;
        char *str = client.createRequest(Request::RequestType::OUTPUT, "(4)");
        int pid;
        memcpy(&pid, str + 4, 4);
        BOOST_CHECK_EQUAL(pid, 1234);
    }

    BOOST_AUTO_TEST_CASE (get_type) {
        Client client;
        char *str = client.createRequest(Request::RequestType::OUTPUT, "(4)");
        Request::RequestType type;
        memcpy(&type, str, 4);
        BOOST_CHECK(type == Request::RequestType::OUTPUT);
    }

    BOOST_AUTO_TEST_CASE (get_datasize) {
        Client client;
        char *str = client.createRequest(Request::RequestType::OUTPUT, "(4)");
        int size;
        memcpy(&size, str + 8, 4);
        BOOST_CHECK_EQUAL(size, 3);
    }

    BOOST_AUTO_TEST_CASE (get_data) {
        Client client;
        char *str = client.createRequest(Request::RequestType::OUTPUT, "(4)");
        int size;
        memcpy(&size, str + 8, 4);
        char *data = new char[size];
        memcpy(data, str + 12, size);
        char result[] = "(4)";
        BOOST_CHECK((strcmp(result, data) == 0));
    }

BOOST_AUTO_TEST_SUITE_END()


