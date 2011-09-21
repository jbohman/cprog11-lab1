#include <cxxtest/TestSuite.h>
#include <stdexcept>
#include <climits>

#include "vector.h"

// copied from exercise 1.4, then modified and improved
class VectorTestSuite : public CxxTest::TestSuite 
{
private:
    
    template <typename T> T access_by_value(Vector<T> v, size_t index) { return v[index]; }
    template <typename T> void change_by_value(Vector<T> v, size_t index, T value) { v[index] = value; }
    
public:

    // Testcase 1

    // Tests the different constructors

    void test_1_constructors( void )
    {
        // constructor with parameter
        Vector<double> a(11);
        Vector<double> b(5);
        
        // copy constructor
        access_by_value(a, 1);
        access_by_value(b, 1);
    }
    
    
    // Testcase 2
    
    // Tests assignment to elements
    
    void test_2_assignment(void)
    {
        Vector<double> a(11);
        
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
        Vector<double> a(2);
        a[1] = 2;
        a[0] = 13;
        
        // Test access to non-const vector
        TS_ASSERT_EQUALS(a[0], 13);
        TS_ASSERT_EQUALS(a[1], 2);
        
        // Test access to const vector
        const Vector<double> b = a;
        TS_ASSERT_EQUALS(b[0], 13);
        TS_ASSERT_EQUALS(b[1], 2);
    }
    
    
    // Testcase 4
    
    // Test index access via references
    
    void test_4_index_ref(void)
    {
        Vector<double> a(2);
        a[1] = 2;
        a[0] = 13;
        
        // Test access via reference
        double & ref = a[1];
        TS_ASSERT_EQUALS(ref, 2);
        ref = 5;
        TS_ASSERT_EQUALS(a[0], 13);
        TS_ASSERT_EQUALS(a[1], 5);
        
        // Test access via const reference
        const Vector<double> b = a;
        const unsigned int & constref = b[0];
        TS_ASSERT_EQUALS(constref, 13);
    }
    
    
    
    // Testcase 5
    
    // Test range checks
    
    void test_5_out_of_bounds(void)
    {
        Vector<double> z(0);
        Vector<double> a(1);
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
        const Vector<double> b(1);
        threw = false;
        try { double d = b[1]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
    }
    
    
    // Test 6
    
    // Test that the copy constructor copies everything correctly
    
    void test_6_copy_constructor(void)
    {
        bool threw;
        Vector<double> z(0);
        Vector<double> a(3);
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
        Vector<double> z(0);
        Vector<double> a(3);
        a[0] = 29;
        a[2] = 31;
        
        // Test a non-empty vector
        Vector<double> c = a;
        
        TS_ASSERT_EQUALS(c[0], 29);
        TS_ASSERT_EQUALS(c[2], 31);
        
        threw = false;
        try { double d = c[3]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        
        // Test the empty vector
        c = z;
        threw = false;
        try { double d = c[0]; }
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
        Vector<double> a(3);
        a[0] = 29;
        a[2] = 31;
        
        // Test copy constructor
        change_by_value(a, 0, 37.0);
        change_by_value(a, 2, 39.0);
        
        // Test assignment constructor
        Vector<double> c = a;
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
            Vector<double> a(5);
            for (size_t j = 0; j < 5; j++) {
                // check that the value was initialized to zero
                TS_ASSERT_EQUALS(a[j], 0);
                
                // this value should NOT be used in the new array
                a[j] = 3.1415926535897931;
            }
        }
    }
    
    
    // Test 10
    
    // Test initialization of custom default values.
    
    void test_10_custom_defaults(void)
    {
        double value = 32.16;
        
        Vector<double> a(11, value);
        for (size_t j = 0; j < 5; j++) {
            // check that the value was initialized to zero
            TS_ASSERT_EQUALS(a[j], value);
        }
    }
    
    
    // Test 11
    
    // Test push_back
    
    void test_11_push_back(void)
    {
        Vector<double> a;
        TS_ASSERT_EQUALS(a.size(), 0);
        
        double d = 0.4711;
        for (size_t i = 0; i < 50; i++) {
            // Add one element
            a.push_back(d);
            
            TS_ASSERT_EQUALS(a[i], d);
            TS_ASSERT_EQUALS(a.size(), i+1);
            d *= 3.1416;
        }
    }
    
    
    // Test 12
    
    // Test insert
    
    void test_12_insert(void)
    {
        Vector<double> a;
        
        // Insert to an empty list
        a.insert(0, 1.23);
        TS_ASSERT_EQUALS(a.size(), 1);
        TS_ASSERT_EQUALS(a[0], 1.23);
        
        // Insert first
        a.insert(0, 2.34);
        TS_ASSERT_EQUALS(a.size(), 2);
        TS_ASSERT_EQUALS(a[0], 2.34);
        TS_ASSERT_EQUALS(a[1], 1.23);
        
        // Insert last
        a.insert(2, 3.45);
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT_EQUALS(a[0], 2.34);
        TS_ASSERT_EQUALS(a[1], 1.23);
        TS_ASSERT_EQUALS(a[2], 3.45);
        
        // Insert in the middle
        a.insert(1, 4.56);
        TS_ASSERT_EQUALS(a.size(), 4);
        TS_ASSERT_EQUALS(a[0], 2.34);
        TS_ASSERT_EQUALS(a[1], 4.56);
        TS_ASSERT_EQUALS(a[2], 1.23);
        TS_ASSERT_EQUALS(a[3], 3.45);
        
        // Insert out of bounds
        bool threw = false;
        try { a.insert(5, 5.67); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        TS_ASSERT_EQUALS(a.size(), 4);
    }
    
    
    // Test 13
    
    // Test erase
    
    void test_13_erase(void)
    {
        bool threw;
        Vector<double> a(2);
        a[0] = 1.23;
        
        // Erase last element
        a.erase(1);
        TS_ASSERT_EQUALS(a.size(), 1);
        TS_ASSERT_EQUALS(a[0], 1.23);
        
        // Erase the only remaining element
        a.erase(0);
        TS_ASSERT_EQUALS(a.size(), 0);
        
        // Try to erase in an empty vector
        threw = false;
        try { a.erase(0); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        TS_ASSERT_EQUALS(a.size(), 0);
        
        // Erase the first element
        a.push_back(2.34);
        a.push_back(3.45);
        a.erase(0);
        TS_ASSERT_EQUALS(a.size(), 1);
        TS_ASSERT_EQUALS(a[0], 3.45);
        
        // Erase middle element
        a.push_back(4.56);
        a.push_back(5.67);
        a.erase(1);
        TS_ASSERT_EQUALS(a.size(), 2);
        TS_ASSERT_EQUALS(a[0], 3.45);
        TS_ASSERT_EQUALS(a[1], 5.67);
        
        // Check bounds checking
        threw = false;
        try { a.erase(2); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT_EQUALS(threw, true);
        TS_ASSERT_EQUALS(a.size(), 2);
    }
};

