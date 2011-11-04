#ifndef MATRIX_TEST_H_INCLUDED
#define MATRIX_TEST_H_INCLUDED

#include "Matrix.h"
#include "/info/cprog09/cxxtest/cxxtest/TestSuite.h"
#include <fstream>
#include <sstream>

class MatrixTestSuite : public CxxTest::TestSuite
{

    Matrix a_matrix_3by2() {    // [ 1 3 5 ]
        Matrix m;               // [ 0 2 0 ]
        std::stringstream s("  [ 1 3 5 ; 0 2 0 ]");
        s >> m;
        return m;
    }
    
    Matrix matrix_3by3() {
        return strmat("  [ 3 1 4 ; 1 5 9 ; 0 0 0 ]");
    }
    
    Matrix strmat(const char *str) {
        Matrix m;
        std::stringstream s(str);
        s >> m;
        return m;
    }

    void init_matrix( Matrix& m, const char* file )
    {
        std::stringstream stream( file );   
        stream >> m;
    }

public:
    void testIndexOperator ( )
    {
        Matrix m( 2, 2 );
        TS_ASSERT( m[ 0 ][ 1 ] == 0 );

        m = a_matrix_3by2();
        TS_ASSERT( m[ 0 ][ 0 ] == 1 );

        init_matrix(m, "  [ 1 3 5 ; 0 2 1 ]");
        TS_ASSERT( m[ 0 ][ 0 ] == 1 );

        std::stringstream ss;
        ss << m;
        ss >> m;
        TS_ASSERT( m[ 0 ][ 0 ] == 1 );
    }
    
    /* Test constructors */
    void testConstructors()
    {
        Matrix a;
        TS_ASSERT(a.rows() == 0 && a.cols() == 0);
        
        Matrix b(3, 2);
        TS_ASSERT(b.rows() == 3 && b.cols() == 2);
        check_zero(b);
        
        Matrix c(5);
        TS_ASSERT(c.rows() == 5 && c.cols() == 5);
        check_identity(c);
        
        Matrix d(3, 3);
        TS_ASSERT(d.rows() == 3 && d.cols() == 3);
        check_zero(d);
    }
    
    void testCopy()
    {
        Matrix a = a_matrix_3by2();
        Matrix b = a;
        check_equal(a, b);
        
        Matrix c;
        Matrix d = c;
        check_equal(c, d);
    }
    
    void testAssign()
    {
        Matrix a = a_matrix_3by2();
        Matrix b(1, 0);
        b = a;
        check_equal(a, b);
        
        Matrix c;
        Matrix d(1, 4);
        d = c;
        check_equal(c, d);
        
        Matrix e(3, 5);
        Matrix f;
        f = e;
        check_equal(e, f);
    }
    
    void testSetIndexOperator()
    {
        Matrix a(3);
        
        Matrix m = matrix_3by3();
        
        check_not_equal(m, a);
        
        a[0][0] = 3;
        a[0][1] = 1;
        a[0][2] = 4;
        a[1][0] = 1;
        a[1][1] = 5;
        a[1][2] = 9;
        a[2][0] = 0;
        a[2][1] = 0;
        a[2][2] = 0;
        
        check_equal(m, a);
        
        a[2][2] = 1;
        check_not_equal(m, a);
        
        m[2][2] = 1;
        check_equal(m, a);
        
        TS_ASSERT_THROWS_ANYTHING(m[-1]);
        TS_ASSERT_THROWS_ANYTHING(m[3]);
        TS_ASSERT_THROWS_ANYTHING(m[0][-1]);
        TS_ASSERT_THROWS_ANYTHING(m[0][3]);
    }
    
    void testPlus() {
        Matrix a =          strmat("  [ 1 2 3 ; 4 5 6 ; 7 8 9 ]");
        Matrix b = matrix_3by3(); // [ 3 1 4 ; 1 5 9 ; 0 0 0 ]
        
        Matrix c = a + b;
        Matrix res = strmat("  [ 4 3 7 ; 5 10 15 ; 7 8 9 ]");
        
        check_equal(c, res);
        
        Matrix d = b + a;
        check_equal(d, res);
        
        Matrix e = a_matrix_3by2();
        TS_ASSERT_THROWS_ANYTHING(e + b);
        TS_ASSERT_THROWS_ANYTHING(b + e);
    }
    
    void testProduct() {
        Matrix a = strmat("  [ 1 2 3; 4 5 6 ]");
        Matrix b = strmat("  [ 9; 8; 7 ]");
        
        Matrix c = a*b;
        check_equal(c, strmat("  [ 46; 118 ]"));
        TS_ASSERT_THROWS_ANYTHING(b*a);
        
        Matrix empty;
        TS_ASSERT_THROWS_ANYTHING(a*empty);
        TS_ASSERT_THROWS_ANYTHING(empty*a);
        
        check_equal(empty, empty*empty);
    }

private:
    void check_zero(const Matrix & m)
    {
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                TS_ASSERT_EQUALS(m[r][c], 0);
            }
        }
    }
    
    void check_identity(const Matrix & m)
    {
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                TS_ASSERT_EQUALS(m[r][c], r == c ? 1 : 0);
            }
        }
    }
    
    void check_equal(const Matrix & m, const Matrix & n)
    {
        TS_ASSERT_EQUALS(m.rows(), n.rows());
        TS_ASSERT_EQUALS(m.cols(), n.cols());
        
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                TS_ASSERT_EQUALS(m[r][c], n[r][c]);
            }
        }
    }
    
    void check_not_equal(const Matrix & m, const Matrix & n)
    {
        if (m.rows() != n.rows()) return;
        if (m.cols() != n.cols()) return;
        
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                if (m[r][c] != n[r][c]) return;
            }
        }
        TS_ASSERT(false);
    }
};

#endif

