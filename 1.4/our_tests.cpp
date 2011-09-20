#include <cxxtest/TestSuite.h>

#include "vector.h"

class VectorTestSuite : public CxxTest::TestSuite 
{
private:
    
    void call_by_value(Vector v) { }
    
public:

    // Testcase 1

    // Tests the different constructors

    void test_1_constructors( void )
    {
        // constructor with parameter
        Vector a(11);
        Vector b(5);
        
        // copy constructor
        call_by_value(a);
        call_by_value(b);
        
        // assignment constructor
        Vector c = a;
        c = b;
        a = a;
        b = b;
    }
    
    
    // Testcase 2
    
    // Tests assignment to elements
    
    void test_2_assignment(void)
    {
        Vector a(11);
        
        for (size_t i = 0; i < 11; i++) {
            a[i] = 7*i;
        }
        
        for (size_t i = 0; i < 11; i++) {
            TS_ASSERT_EQUALS(a[i], 7*i);
        }
    }
    
    
    // Testcase 3
    
    // Tests index access
    
    void test_3_index(void)
    {
        Vector a(2);
        a[1] = 2;
        a[0] = 13;
        
        // Test access to non-const vector
        TS_ASSERT_EQUALS(a[0], 13);
        TS_ASSERT_EQUALS(a[1], 2);
        
        // Test access to const vector
        const Vector b = a;
        TS_ASSERT_EQUALS(b[0], 13);
        TS_ASSERT_EQUALS(b[1], 2);
    }
    
    
    // Testcase 4
    
    // Test index access via references
    
    void test_4_index_ref(void)
    {
        Vector a(2);
        a[1] = 2;
        a[0] = 13;
        
        // Test access via reference
        unsigned int & ref = a[1];
        TS_ASSERT_EQUALS(ref, 2);
        ref = 5;
        TS_ASSERT_EQUALS(a[0], 13);
        TS_ASSERT_EQUALS(a[1], 5);
        
        // Test access via const reference
        const Vector b = a;
        const unsigned int & constref = b[0];
        TS_ASSERT_EQUALS(constref, 13);
    }
    
};

