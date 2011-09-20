#include <cxxtest/TestSuite.h>
#include <stdexcept>
#include <climits>

#include "vector.h"

class VectorTestSuite : public CxxTest::TestSuite 
{
private:
    
    int access_by_value(Vector v, size_t index) { return v[index]; }
    void change_by_value(Vector v, size_t index, int value) { v[index] = value; }
    
public:

    // Testcase 1

    // Tests the different constructors

    void test_1_constructors( void )
    {
        // constructor with parameter
        Vector a(11);
        Vector b(5);
        
        // copy constructor
        access_by_value(a, 1);
        access_by_value(b, 1);
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
    
    
    
    // Testcase 5
    
    // Test range checks
    
    void test_5_out_of_bounds(void)
    {
        Vector z(0);
        Vector a(1);
        bool threw = false;
        
        // Test with an empty vector
        try { z[0] = 17; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test negative index
        threw = false;
        try { a[-1] = 17; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test too large index
        threw = false;
        try { a[1] = 17; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test with a const vector too
        const Vector b(1);
        threw = false;
        try { int i = b[1]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
    }
    
    
    // Test 6
    
    // Test that the copy constructor copies everything correctly
    
    void test_6_copy_constructor(void)
    {
        bool threw;
        Vector z(0);
        Vector a(3);
        a[0] = 23;
        a[2] = 19;
        
        // Test copying of an empty vector
        threw = false;
        try { access_by_value(z, 0); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test copying of a non-empty vector
        TS_ASSERT_EQUALS(access_by_value(a, 0), 23);
        TS_ASSERT_EQUALS(access_by_value(a, 2), 19);
        
        // Check that the size is still right
        threw = false;
        try { access_by_value(a, 3); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
    }
    
    
    // Test 7
    
    // Test assignment operator
    
    void test_7_assignment_operator(void)
    {
        bool threw;
        Vector z(0);
        Vector a(3);
        a[0] = 29;
        a[2] = 31;
        
        // Test a non-empty vector
        Vector c = a;
        
        TS_ASSERT_EQUALS(c[0], 29);
        TS_ASSERT_EQUALS(c[2], 31);
        
        threw = false;
        try { int i = c[3]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test the empty vector
        c = z;
        threw = false;
        try { int i = c[0]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test self-assignment
        z = z;
        a = a;
        c = c;
    }
    
    
    // Test 8
    
    // Test that changes to copies don't affect the originals
    
    void test_8_changes_to_copies(void)
    {
        Vector a(3);
        a[0] = 29;
        a[2] = 31;
        
        // Test copy constructor
        change_by_value(a, 0, 37);
        change_by_value(a, 2, 39);
        
        // Test assignment constructor
        Vector c = a;
        c[0] = 37;
        a[2] = 39;
        
        TS_ASSERT_EQUALS(a[0], 29);
        TS_ASSERT_EQUALS(c[2], 31);
    }
    
    
    // Test 9
    
    // Test initialization of default values. This test is more reliable if
    // it's run under Valgrind.
    
    void test_9_defaults(void)
    {
        for (size_t i = 0; i < 2; i++) {
            // Test twice. Hopefully the vector is allocated in the same place
            // the second time.
            Vector a(5);
            for (size_t j = 0; j < 5; j++) {
                // check that the value was initialized to zero
                TS_ASSERT_EQUALS(a[j], 0);
                
                // set all bits to one
                a[j] = UINT_MAX;
            }
        }
    }
};

