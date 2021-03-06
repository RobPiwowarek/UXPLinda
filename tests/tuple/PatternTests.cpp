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

    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_string_wild_chart_pattern_inside){
        Pattern pattern = Pattern("String:\"ab*d\"");
        Tuple tuple =  Tuple("(\"abcdh\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_string_wild_chart_pattern_inside){
        Pattern pattern = Pattern("String:\"ab*d\"");
        Tuple tuple =  Tuple("(\"abcd\")");
        Tuple tuple1 =  Tuple("(\"abdd\")");
        BOOST_CHECK( pattern.match(&tuple) );
        BOOST_CHECK( pattern.match(&tuple1) );
    }
    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_string_wild_chart_pattern_atTheBeginning){
        Pattern pattern = Pattern("String:\"*ab\"");
        Tuple tuple =  Tuple("(\"sc\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_string_wild_chart_pattern_atTheBeginning){
        Pattern pattern = Pattern("String:\"*ab\"");
        Tuple tuple =  Tuple("(\"afsab\")");
        BOOST_CHECK( pattern.match(&tuple) );
    }
    BOOST_AUTO_TEST_CASE (dont_match_incorrect_tuple_with_many_string_wild_chart_pattern){
        //brakuje e na koncu a przechodzi
        Pattern pattern = Pattern("String:\"a*bcd*e\"");
        Tuple tuple =  Tuple("(\"aabcdWWWW\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_many_string_wild_chart_pattern){
        Pattern pattern = Pattern("String:\"ab*cd*e\"");
        Tuple tuple =  Tuple("(\"abcdWWe\")");
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

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_with_many_string_pattern){
        Pattern pattern = Pattern("String:\"5\", String:*");
        Tuple tuple =  Tuple("(\"5\", \"1\")");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_tuple_with_with_many_string_pattern){
        Pattern pattern = Pattern("String:\"5\", String:*");
        Tuple tuple =  Tuple("(\"4\", \"1\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_tuple_with_with_many_string_pattern_2){
        Pattern pattern = Pattern("String:\"5\", String:\"4\"");
        Tuple tuple =  Tuple("(\"5\", \"1\")");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (match_correct_tuple_with_with_both_types_pattern){
        Pattern pattern = Pattern("Integer:5, String:*");
        Tuple tuple =  Tuple("(5, \"1\")");
        BOOST_CHECK( pattern.match(&tuple) );
    }

    BOOST_AUTO_TEST_CASE (dont_match_tuple_with_with_both_types_pattern){
        Pattern pattern = Pattern("String:\"5\", Integer:*");
        Tuple tuple =  Tuple("(\"4\", 1)");
        BOOST_CHECK( ! pattern.match(&tuple) );
    }


BOOST_AUTO_TEST_SUITE_END()