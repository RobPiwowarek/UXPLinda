#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/tuple/Tuple.h"

BOOST_AUTO_TEST_SUITE (PatternsWithIntTests)

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_integer_greater_pattern){
        Pattern pattern = Pattern("Integer:>5");
        Tuple tuple =  Tuple("(1)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_integer_greater_pattern){
        Pattern pattern = Pattern("Integer:>5");
        Tuple tuple =  Tuple("(6)");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_integer_equal_pattern){
        Pattern pattern = Pattern("Integer:5");
        Tuple tuple =  Tuple("(1)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_integer_equal_pattern){
        Pattern pattern = Pattern("Integer:5");
        Tuple tuple =  Tuple("(5)");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_integer_lowerEqual_pattern){
        Pattern pattern = Pattern("Integer:<=5");
        Tuple tuple =  Tuple("(6)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_integer_lowerEqual_pattern){
        Pattern pattern = Pattern("Integer:<=5");
        Tuple tuple =  Tuple("(1)");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_integer_equal_sign_pattern){
        Pattern pattern = Pattern("Integer:==5");
        Tuple tuple =  Tuple("(1)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_integer_equal_sign_pattern){
        Pattern pattern = Pattern("Integer:==5");
        Tuple tuple =  Tuple("(5)");
        BOOST_CHECK( pattern.match(&tuple) );
    }
    BOOST_AUTO_TEST_CASE (match_int_tuple_with_int_wild_chart_pattern){
        Pattern pattern = Pattern("Integer:*");
        Tuple tuple =  Tuple("(1)");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_string_tuple_with_int_wild_chart_pattern){
        Pattern pattern = Pattern("Integer:*");
        Tuple tuple =  Tuple("(\"5\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_with_many_integers_pattern){
        Pattern pattern = Pattern("Integer:<=5, Integer:1");
        Tuple tuple =  Tuple("(5, 1)");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_with_many_integers_pattern){
        Pattern pattern = Pattern("Integer:<=5, Integer: 1");
        Tuple tuple =  Tuple("(6,1)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE (PatternsWithStringTests)

    BOOST_AUTO_TEST_CASE (dont_match_pattern_with_incorrect_string_tuple){
        Pattern pattern = Pattern("String:\"123\"");
        Tuple tuple =  Tuple("(\"1234\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_pattern_with_correct_string_tuple) {
        Pattern pattern = Pattern("String:\"123\"");
        Tuple tuple = Tuple("(\"123\")");
        BOOST_CHECK(pattern.match(&tuple));
    }

    BOOST_AUTO_TEST_CASE (dont_match_pattern_with_correct_int_tuple){
        Pattern pattern = Pattern("String:\"123\"");
        Tuple tuple =  Tuple("(1234)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_string_wild_chart_pattern){
        Pattern pattern = Pattern("String:\"ab*\"");
        Tuple tuple =  Tuple("(\"aabc\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_string_wild_chart_pattern){
        Pattern pattern = Pattern("String:\"ab*\"");
        Tuple tuple =  Tuple("(\"abc\")");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_empty_string_pattern_with_string){
        Pattern pattern = Pattern("String:*");
        Tuple tuple =  Tuple("(\"abc\")");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_empty_string_pattern_with_int){
        Pattern pattern = Pattern("String:*");
        Tuple tuple =  Tuple("(5)");
        BOOST_CHECK( !pattern.match(&tuple) );
    }

BOOST_AUTO_TEST_SUITE_END()