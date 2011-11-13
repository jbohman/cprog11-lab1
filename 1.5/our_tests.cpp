#include <cxxtest/TestSuite.h>
#include <stdexcept>
#include <climits>
#include <string>

#include "vector.h"

// Used for testing sort()
class Date
{
    int year, month, day;
public:
    Date() : year(0), month(0), day(0) { }
    Date(int y, int m, int d) : year(y), month(m), day(d) { }
    bool operator<(const Date & other) const {
        return (year < other.year || month < other.month || day < other.day);
    }
};

// Used for testing destructor calling

static int num_destructible_alive; // number of objects that haven't been deleted yet

class Destructible
{
    int status; // == 1234 if still allocated
public:
    
    Destructible() : status(1234) {
        num_destructible_alive++;
    }
    
    Destructible(const Destructible & orig) : status(1234) {
        num_destructible_alive++;
    }
    
    // should only be used for overwriting alive objects with other alive objects
    Destructible & operator=(const Destructible & from) {
        TS_ASSERT_EQUALS(status, 1234);
        return *this;
    }
    
    ~Destructible() {
        TS_ASSERT_EQUALS(status, 1234);
        status = 4321;
        num_destructible_alive--;
    }
};

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
        TS_ASSERT(threw);
        
        // Test negative index
        threw = false;
        try { a[-1] = 17; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT(threw);
        
        // Test too large index
        threw = false;
        try { a[1] = 17; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT(threw);
        
        // Test with a const vector too
        const Vector<double> b(1);
        threw = false;
        try { double d = b[1]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT(threw);
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
        TS_ASSERT(threw);
        
        // Test copying of a non-empty vector
        TS_ASSERT_EQUALS(access_by_value(a, 0), 23);
        TS_ASSERT_EQUALS(access_by_value(a, 2), 19);
        
        // Check that the size is still right
        threw = false;
        try { access_by_value(a, 3); }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT(threw);
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
        TS_ASSERT(threw);
        
        // Test the empty vector
        c = z;
        threw = false;
        try { double d = c[0]; }
        catch (std::out_of_range e) { threw = true; }
        TS_ASSERT(threw);
        
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
        for (size_t j = 0; j < 11; j++) {
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
        TS_ASSERT(threw);
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
        TS_ASSERT(threw);
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
        TS_ASSERT(threw);
        TS_ASSERT_EQUALS(a.size(), 2);
    }
    
    
    // Test 14
    
    // Test clear
    
    void test_14_clear(void)
    {
        Vector<double> a(5);
        a.clear();
        TS_ASSERT_EQUALS(a.size(), 0);
    }
    
    
    // Test 15
    
    // Test sort
    
    void test_15_sort(void)
    {
        Vector<double> a(3);
        
        // Sort ascending with 3 elements
        a[0] = 3.45;
        a[1] = 4.56;
        a[2] = 1.23;
        a.sort();
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT_EQUALS(a[0], 1.23);
        TS_ASSERT_EQUALS(a[1], 3.45);
        TS_ASSERT_EQUALS(a[2], 4.56);
        
        // Sort descending with 3 elements
        a.sort(false);
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT_EQUALS(a[0], 4.56);
        TS_ASSERT_EQUALS(a[1], 3.45);
        TS_ASSERT_EQUALS(a[2], 1.23);
        
        // Sort after elements have been removed
        a.erase(0);
        a.sort(true);
        TS_ASSERT_EQUALS(a[0], 1.23);
        TS_ASSERT_EQUALS(a[1], 3.45);
        
        // Sort an empty vector
        a.clear();
        
        a.sort();
        TS_ASSERT_EQUALS(a.size(), 0);
        
        a.sort(false);
        TS_ASSERT_EQUALS(a.size(), 0);
        
        a.sort(true);
        TS_ASSERT_EQUALS(a.size(), 0);
        
        // Sort a type which only implements < (and not ==, <=, >, >=)
        Vector<Date> dates;
        dates.push_back(Date(2002,  2, 22));
        dates.push_back(Date(1999, 12, 31));
        dates.push_back(Date(2011,  9, 24));
        dates.push_back(Date(2000,  1,  1));
        
/*        dates.sort();
        TS_ASSERT_EQUALS(dates.size(), 4);
        TS_ASSERT(dates[0] < dates[1]);
        TS_ASSERT(dates[1] < dates[2]);
        TS_ASSERT(dates[2] < dates[3]);*/
        
        dates.sort(false);
        TS_ASSERT_EQUALS(dates.size(), 4);
        TS_ASSERT(dates[3] < dates[2]);
        TS_ASSERT(dates[2] < dates[1]);
        TS_ASSERT(dates[1] < dates[0]);
    }
    
    
    // Testcase 16
    
    // Test with vectors as elements
    
    void test_16_vector_as_elements(void)
    {
        // Create some vectors that will be used as elements
        Vector<double> v[3];
        v[0].push_back(5);
        v[2].push_back(4711);
        v[2].push_back(42);
        
        // Add these to a vector
        Vector<Vector<double> > a;
        for (int i = 0; i < 3; i++) {
            a.push_back(v[i]);
        }
        
        TS_ASSERT_EQUALS(a.size(), 3);
        TS_ASSERT(compare_vectors(a[0], v[0]));
        TS_ASSERT(compare_vectors(a[1], v[1]));
        TS_ASSERT(compare_vectors(a[2], v[2]));
        
        // Copy with copy constructor
        Vector<Vector<double> > b(a);
        TS_ASSERT_EQUALS(b.size(), 3);
        TS_ASSERT(compare_vectors(b[0], v[0]));
        TS_ASSERT(compare_vectors(b[1], v[1]));
        TS_ASSERT(compare_vectors(b[2], v[2]));
        
        // Add stuff to this copy
        b.push_back(Vector<double>(11));
        TS_ASSERT_EQUALS(b.size(), 4);
        TS_ASSERT_EQUALS(b[3].size(), 11);
        
        // Copy with assignment
        Vector<Vector<double> > c;
        c.push_back(Vector<double>(5)); // put some data there that will be replaced
        c = b;
        TS_ASSERT_EQUALS(c.size(), 4);
        TS_ASSERT(compare_vectors(c[0], v[0]));
        TS_ASSERT(compare_vectors(c[1], v[1]));
        TS_ASSERT(compare_vectors(c[2], v[2]));
        TS_ASSERT_EQUALS(c[3].size(), 11);
        
        // Add stuff to this copy
        c.push_back(Vector<double>(17));
        TS_ASSERT_EQUALS(c.size(), 5);
        TS_ASSERT_EQUALS(c[4].size(), 17);
    }
    
    
    // Test 17
    
    // Try to trigger bugs with internal_size vs. capacity.
    
    void test_17_capacity(void)
    {
        Vector<double> a(1000);
        
        // Trigger a resize internally
        a.push_back(1.23);
        a.push_back(2.34);
        a.push_back(3.45);
        TS_ASSERT_EQUALS(a.size(), 1003);
        
        // See if the assignment operator handles the extra capacity correctly
        Vector<double> b(3);
        b = a;
        TS_ASSERT_EQUALS(b.size(), 1003);
        
        for (size_t i = 0; i < 2000; i++) {
            double d = 4.56*i;
            b.push_back(d);
            TS_ASSERT_EQUALS(b[1003+i], d);
        }
        
        // See if the assignment operator handles the extra capacity correctly
        Vector<double> c(a);
        TS_ASSERT_EQUALS(c.size(), 1003);
        
        for (size_t i = 0; i < 2000; i++) {
            double d = 4.56*i;
            c.push_back(d);
            TS_ASSERT_EQUALS(c[1003+i], d);
        }
    }
    
    
    // Test 18
    
    // Test copying and assignment with empty vectors
    
    void test_18_copy_empty(void)
    {
        Vector<double> a;
        Vector<double> b(0);
        Vector<double> c(10);
        c.push_back(1.23);
        c.push_back(2.34);
        c.clear();
        
        // We should now have three empty vectors
        TS_ASSERT_EQUALS(a.size(), 0);
        TS_ASSERT_EQUALS(b.size(), 0);
        TS_ASSERT_EQUALS(c.size(), 0);
        
        // Copy and assign these
        Vector<double> d(a);
        Vector<double> e(b);
        Vector<double> f(c);
        d = b;
        d = f;
        e = d;
        Vector<double> g(e);
        TS_ASSERT_EQUALS(g.size(), 0);
        
        // Check that the new vector is working
        g.sort();
        TS_ASSERT_EQUALS(g.size(), 0);
        
        g.push_back(3.45);
        TS_ASSERT_EQUALS(g.size(), 1);
    }
    
    
    // Test 19
    
    // Checks that destructors, assignment operators, etc. of the elements
    // are called properly.
    
    void test_19_destructors(void)
    {
        TS_ASSERT_EQUALS(num_destructible_alive, 0);
        
        { // extra scope
            
            Vector<Destructible> a;
            a.push_back(Destructible());
            
            Vector<Destructible> b(a);
            b.push_back(Destructible());
            
            b = a;
            a.clear();
            
            b = a;
            
            // Check extra capacity elements
            Vector<Destructible> g(1000);
            for (size_t i = 0; i < 50; i++) { // trigger a resize
                g.push_back(Destructible());
            }
            
        } // all objects in the scope are destroyed here
        
        TS_ASSERT_EQUALS(num_destructible_alive, 0);
    }
    
    
    // Test 20
    
    // Strings
    
    void test_20_strings(void)
    {
        Vector<std::string> a, b;
        for (int i = 0; i < 5000; i++) {
            
            int which = rand() % 2;
            Vector<std::string> & v = (which ? a : b);
            Vector<std::string> & w = (which ? b : a);
            
            int op = rand() % 5;
            switch (op) {
                case 0:
                    v.push_back("hej");
                    break;
                case 1:
                    try { v.insert(rand() % 200, "tjo"); }
                    catch (std::out_of_range) { }
                case 2:
                    try { v.erase(rand() % 200); }
                    catch (std::out_of_range) { }
                case 3:
                    w = v;
                    break;
                case 4:
                    Vector<std::string> c(v);
                    break;
            }
            
            if (rand() % 300 == 0) v.clear();
        }
    }

    // Test 21

    // Test resize
    void test_21_resize()
    {
        Vector<int> v;
        
        v.resize(2);
        TS_ASSERT_EQUALS(v.size(), 2);
        TS_ASSERT_EQUALS(v[0], 0);
        TS_ASSERT_EQUALS(v[1], 0);
        
        v[0] = 3;
        v[1] = 5;
        v.resize(1);
        v.resize(3);
        TS_ASSERT_EQUALS(v.size(), 3);
        TS_ASSERT_EQUALS(v[0], 3);
        TS_ASSERT_EQUALS(v[1], 0);
        TS_ASSERT_EQUALS(v[2], 0);
        
        v.resize(100, 17);
        TS_ASSERT_EQUALS(v.size(), 100);
        TS_ASSERT_EQUALS(v[0], 3);
        TS_ASSERT_EQUALS(v[1], 0);
        TS_ASSERT_EQUALS(v[2], 0);
        TS_ASSERT_EQUALS(v[3], 17);
        TS_ASSERT_EQUALS(v[99], 17);
    }
};


