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
    
};

