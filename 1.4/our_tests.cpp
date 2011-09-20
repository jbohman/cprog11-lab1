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
    
};

