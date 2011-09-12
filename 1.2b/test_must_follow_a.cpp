#include <cxxtest/TestSuite.h>

#include "must_follow_a.h"

class MyTestSuite : public CxxTest::TestSuite 
{
public:

    // Testcase 1

    // This testcase sets up a 5 sized array (precondition). Note that
    // the second argument (length) to must_follow_a is 4. The
    // expected result is successs.

    // Do make additional tests of your own and try it out. 

    void test_a_is_second_to_last( void )
    {
        char vek[] = {'x', 'x', 'a', 'b', 'x'};
        int result = must_follow_a(vek, 4, 'a', 'b');
        TS_ASSERT_EQUALS( result, 1);
    }
    
    // Testcase 2

    // Checks string with a repeated pattern with two characters.

    void test_b_repeated_twice( void )
    {
        char vek[] = {'a', 'b', 'a', 'b', 'x'};
        int result = must_follow_a(vek, 4, 'a', 'b');
        TS_ASSERT_EQUALS(result, 2);
    }
    
    // Testcase 3

    // Checks off-by-one problem with length.

    void test_c_length_off_by_one( void )
    {
        char vek[] = {'b', 'b', 'a', 'b', 'b'};
        int result = must_follow_a(vek, 3, 'a', 'b');
        TS_ASSERT_EQUALS(result, 0);
    }
    
    // Testcase 4

    // Tests with an empty vector.

    void test_d_( void )
    {
        char vek[] = {};
        int result = must_follow_a(vek, 0, 'a', 'b');
        TS_ASSERT_EQUALS(result, 0);
    }
};
