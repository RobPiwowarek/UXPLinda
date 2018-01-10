#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/tuple/TupleSpace.h"

BOOST_AUTO_TEST_SUITE (TupleSpaceTests)

    BOOST_AUTO_TEST_CASE (tuple_space_is_empty_return_null_when_input){
        TupleSpace space;
        Pattern pattern = Pattern("Integer:>5");
        BOOST_CHECK( space.input(pattern) == nullptr );
    }

    BOOST_AUTO_TEST_CASE (tuple_space_is_empty_return_null_when_read){
        TupleSpace space;
        Pattern pattern = Pattern("Integer:>5");
        BOOST_CHECK( space.read(pattern) == nullptr );
    }
    BOOST_AUTO_TEST_CASE (return_true_when_getting_a_new_tuple){
        TupleSpace space;
        Tuple tuple = Tuple("(6)");
        BOOST_TEST( space.output(tuple) == true );
    }

    BOOST_AUTO_TEST_CASE (missing_matching_tuple_return_null_when_input){
        TupleSpace space;
        Pattern pattern = Pattern("Integer:<5");
        BOOST_CHECK( space.input(pattern) == nullptr );
    }

    BOOST_AUTO_TEST_CASE (missing_matching_tuple_return_null_when_read){
        TupleSpace space;
        Pattern pattern = Pattern("Integer:<5");
        BOOST_CHECK( space.read(pattern) == nullptr  );
    }

    BOOST_AUTO_TEST_CASE (matching_tuple_in_tuplespace_return_tuple_when_input){
        TupleSpace space;
        Tuple tuple = Tuple("(6)");
        space.output(tuple);
        Pattern pattern = Pattern("Integer:>5");
        BOOST_CHECK( *space.input(pattern) == tuple );
    }

    BOOST_AUTO_TEST_CASE (matching_tuple_in_tuplespace_return_tuple_when_read){
        TupleSpace space;
        Tuple tuple = Tuple("(6)");
        space.output(tuple);
        Pattern pattern = Pattern("Integer:>5");
        BOOST_CHECK( *space.input(pattern) == tuple );
    }

    BOOST_AUTO_TEST_CASE (matching_tuple_was_taken_by_input_return_null){
        TupleSpace space;
        Tuple tuple = Tuple("(6)");
        space.output(tuple);
        Pattern pattern = Pattern("Integer:>5");
        space.input(pattern);
        BOOST_CHECK( space.input(pattern) == nullptr );
        BOOST_CHECK( space.read(pattern) == nullptr );
    }

    BOOST_AUTO_TEST_CASE (matching_tuple_taken_by_read_should_be_in_tuplespace){
        TupleSpace space;
        Tuple tuple = Tuple("(6)");
        space.output(tuple);
        Pattern pattern = Pattern("Integer:>5");
        space.read(pattern);
        BOOST_CHECK( *space.read(pattern) == tuple );
        BOOST_CHECK( *space.input(pattern) == tuple );
    }
BOOST_AUTO_TEST_SUITE_END()


