#include <cxxtest/TestSuite.h>
#include <stdexcept>

#include "bitvector.h"

// copied from exercise 1.4, then modified and improved
class VectorTestSuite : public CxxTest::TestSuite 
{
private:
    
    template <typename T> T access_by_value(Vector<T> v, size_t index) { return v[index]; }
    template <typename T> void change_by_value(Vector<T> v, size_t index, T value) { v[index] = value; }
    
    template <typename T> bool compare_vectors(const Vector<T> & a, const Vector<T> & b)
    {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }
    
public:

    // Testcase 1

    // Tests the different constructors

    void test_1_constructors( void )
    {
        // constructor with parameter
        Vector<bool> a(11);
        Vector<bool> b(5);
        
        // copy constructor
        access_by_value(a, 1);
        access_by_value(b, 1);
    }
    
    
    // Testcase 2
    
    // Tests assignment to elements
    
    void test_2_assignment(void)
    {
        Vector<bool> a(11);
        
        for (size_t i = 0; i < 11; i++) {
            a[i] = true;
        }
        
        for (size_t i = 0; i < 11; i++) {
            TS_ASSERT_EQUALS(a[i], true);
        }
    }
    
    
    // Testcase 3
    
    // Tests index access
    
    void test_3_index(void)
    {
        Vector<bool> a(2);
        a[1] = true;
        a[0] = true;
        
        // Test access to non-const vector
        TS_ASSERT_EQUALS(a[0], true);
        TS_ASSERT_EQUALS(a[1], true);
        
        // Test access to const vector
        const Vector<bool> b = a;
        TS_ASSERT_EQUALS(b[0], true);
        TS_ASSERT_EQUALS(b[1], true);
    }
    
    
    // Testcase 4
    
    // Test index access via references
    
    void test_4_index_ref(void)
    {
        Vector<bool> a(2);
        a[1] = true;
        a[0] = true;
        
        // Test access via reference
        //bool & ref = a[1];
        //TS_ASSERT_EQUALS(ref, true);
        //ref = false;
        //TS_ASSERT_EQUALS(a[0], true);
        //TS_ASSERT_EQUALS(a[1], false);
        
        // Test access via const reference
        const Vector<bool> b = a;
        const bool & constref = b[0];
        TS_ASSERT_EQUALS(constref, true);
    }
    
    
    
    // Testcase 5
    
    // Test range checks
    
    void test_5_out_of_bounds(void)
    {
        Vector<bool> z(0);
        Vector<bool> a(1);
       
        // Test empty
        TS_ASSERT_THROWS(z[0] = true, std::out_of_range);
        // Test negative index
        TS_ASSERT_THROWS(a[-1] = true, std::out_of_range);
        // Test too large index
        TS_ASSERT_THROWS(a[1] = true, std::out_of_range);
        // Test with a const vector too
        const Vector<bool> b(1);
        TS_ASSERT_THROWS(bool d = b[1], std::out_of_range);
    }
    
    
    // Test 6
    
    // Test that the copy constructor copies everything correctly
    
    void test_6_copy_constructor(void)
    {
        bool threw;
        Vector<bool> z(0);
        Vector<bool> a(3);
        a[0] = true;
        a[2] = true;
        
        // Test copying of an empty vector
        TS_ASSERT_THROWS(access_by_value(z, 0), std::out_of_range);
        
        // Test copying of a non-empty vector
        TS_ASSERT_EQUALS(access_by_value(a, 0), true);
        TS_ASSERT_EQUALS(access_by_value(a, 2), true);
        
        // Check that the size is still right
        TS_ASSERT_THROWS(access_by_value(a, 3), std::out_of_range);
    }
    
    
    // Test 7
    
    // Test assignment operator
    
    void test_7_assignment_operator(void)
    {
        Vector<bool> z(0);
        Vector<bool> a(3);
        a[0] = true;
        a[2] = true;
        
        // Test a non-empty vector
        Vector<bool> c = a;
        
        TS_ASSERT_EQUALS(c[0], true);
        TS_ASSERT_EQUALS(c[2], true);
        
        TS_ASSERT_THROWS(bool d = c[3], std::out_of_range);
        
        // Test the empty vector
        c = z;
        TS_ASSERT_THROWS(bool d = c[0], std::out_of_range);
        c = z;
        
        // Test self-assignment
        z = z;
        a = a;
        c = c;
    }
    
    
    // Test 8
    
    // Test that changes to copies don't affect the originals
    
    void test_8_changes_to_copies(void)
    {
        Vector<bool> a(3);
        a[0] = true;
        a[2] = true;
        
        // Test copy constructor
        change_by_value(a, 0, true);
        change_by_value(a, 2, true);
        
        // Test assignment constructor
        Vector<bool> c = a;
        c[0] = true;
        a[2] = true;
        
        TS_ASSERT_EQUALS(a[0], true);
        TS_ASSERT_EQUALS(c[2], true);
    }
    
    
    // Test 9
    
    // Test initialization of default values. This test is more reliable if
    // it's run under Valgrind.
    
    void test_9_defaults(void)
    {
        for (size_t i = 0; i < 2; i++) {
            // Test twice. Hopefully the vector is allocated in the same place
            // the second time.
            Vector<bool> a(5);
            for (size_t j = 0; j < 5; j++) {
                // check that the value was initialized to zero
                TS_ASSERT_EQUALS(a[j], false);
                
                // this value should NOT be used in the new array
                a[j] = true;
            }
        }
    }
    
    
    // Test 10
    
    // Test initialization of custom default values.
    
    void test_10_custom_defaults(void)
    {
        Vector<bool> a(11, true);
        for (size_t j = 0; j < 11; j++) {
            // check that the value was initialized to zero
            TS_ASSERT_EQUALS(a[j], true);
        }
    }
    
    
    // Test 11
    
    // Test push_back
    
    void test_11_push_back(void)
    {
        Vector<bool> a;
        TS_ASSERT_EQUALS(a.size(), 0);
        
        for (size_t i = 0; i < 50; i++) {
            // Add one element
            a.push_back(true);
            
            TS_ASSERT_EQUALS(a[i], true);
            TS_ASSERT_EQUALS(a.size(), i+1);
        }
    }
    
    
    // Test 12
    
    // Test clear
    
    void test_12_clear(void)
    {
        Vector<bool> a(5);
        a.clear();
        TS_ASSERT_EQUALS(a.size(), 0);
    }
    
    
    // Test 13
    
    // Test sort
    
    void test_13_sort(void)
    {
        Vector<bool> a(3);
        
        // Sort ascending with 3 elements
        a[0] = true;
        a[1] = false;
        a[2] = true;
        a.sort();
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT_EQUALS(a[0], false);
        TS_ASSERT_EQUALS(a[1], true);
        TS_ASSERT_EQUALS(a[2], true);
        
        // Sort descending with 3 elements
        a.sort(false);
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT_EQUALS(a[0], true);
        TS_ASSERT_EQUALS(a[1], true);
        TS_ASSERT_EQUALS(a[2], false);
        
        // Sort an empty vector
        a.clear();
        
        a.sort();
        TS_ASSERT_EQUALS(a.size(), 0);
        
        a.sort(false);
        TS_ASSERT_EQUALS(a.size(), 0);
        
        a.sort(true);
        TS_ASSERT_EQUALS(a.size(), 0);
    }
    
    
    // Test 14
    
    // Try to trigger bugs with internal_size vs. capacity.
    
    void test_14_capacity(void)
    {
        Vector<bool> a(1000);
        
        // Trigger a resize internally
        a.push_back(true);
        a.push_back(true);
        a.push_back(true);
        TS_ASSERT_EQUALS(a.size(), 1003);
        
        // See if the assignment operator handles the extra capacity correctly
        Vector<bool> b(3);
        b = a;
        TS_ASSERT_EQUALS(b.size(), 1003);
        
        for (size_t i = 0; i < 2000; i++) {
            b.push_back(true);
            TS_ASSERT_EQUALS(b[1003+i], true);
        }
        
        // See if the assignment operator handles the extra capacity correctly
        Vector<bool> c(a);
        TS_ASSERT_EQUALS(c.size(), 1003);
        
        for (size_t i = 0; i < 2000; i++) {
            c.push_back(false);
            TS_ASSERT_EQUALS(c[1003+i], false);
        }
    }
    
    
    // Test 15
    
    // Test copying and assignment with empty vectors
    
    void test_15_copy_empty(void)
    {
        Vector<bool> a;
        Vector<bool> b(0);
        Vector<bool> c(10);
        c.push_back(true);
        c.push_back(true);
        c.clear();
        
        // We should now have three empty vectors
        TS_ASSERT_EQUALS(a.size(), 0);
        TS_ASSERT_EQUALS(b.size(), 0);
        TS_ASSERT_EQUALS(c.size(), 0);
        
        // Copy and assign these
        Vector<bool> d(a);
        Vector<bool> e(b);
        Vector<bool> f(c);
        d = b;
        d = f;
        e = d;
        Vector<bool> g(e);
        TS_ASSERT_EQUALS(g.size(), 0);
        
        // Check that the new vector is working
        g.sort();
        TS_ASSERT_EQUALS(g.size(), 0);
        
        g.push_back(true);
        TS_ASSERT_EQUALS(g.size(), 1);
    }
    
    // Test 16

    // Test resize
    void test_16_resize()
    {
        Vector<bool> v;
        
        v.resize(2);
        TS_ASSERT_EQUALS(v.size(), 2);
        TS_ASSERT_EQUALS(v[0], false);
        TS_ASSERT_EQUALS(v[1], false);
        
        v[0] = true;
        v[1] = true;
        v.resize(1);
        v.resize(3);
        TS_ASSERT_EQUALS(v.size(), 3);
        TS_ASSERT_EQUALS(v[0], true);
        TS_ASSERT_EQUALS(v[1], false);
        TS_ASSERT_EQUALS(v[2], false);
        
        v.resize(100, true);
        TS_ASSERT_EQUALS(v.size(), 100);
        TS_ASSERT_EQUALS(v[0], true);
        TS_ASSERT_EQUALS(v[1], false);
        TS_ASSERT_EQUALS(v[2], false);
        TS_ASSERT_EQUALS(v[3], true);
        TS_ASSERT_EQUALS(v[99], true);
    }
};


