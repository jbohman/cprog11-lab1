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
        
        Matrix sqa(2), sqb(2); // square matrices
        // square a [ -5 2; 3 1 ]
        sqa[0][0] = -5;
        sqa[0][1] = 2;
        sqa[1][0] = 3;
        // square b [ 3 -4; -1 7 ]
        sqb[0][0] = 3;
        sqb[0][1] = -4;
        sqb[1][0] = -1;
        sqb[1][1] = 7;
        Matrix ressquare = strmat("  [ -17 34; 8 -5 ]");
        check_equal(sqa*sqb, ressquare);
        
        // vector*vector
        Matrix v = strmat("  [ 1 2 ]");
        Matrix u = strmat("  [ 3; 4 ]");
        Matrix resvector = strmat("  [ 11 ]");
        check_equal(v*u, resvector);
        
        Matrix empty;
        TS_ASSERT_THROWS_ANYTHING(a*empty);
        TS_ASSERT_THROWS_ANYTHING(empty*a);
        
        check_equal(empty, empty*empty);
    }
    
    void testScalarMultiply() {
        Matrix a = strmat("  [ 1 2 3; 4 5 6 ]");
        
        check_equal(2*a, strmat("  [ 2 4 6; 8 10 12 ]"));
        check_equal(0*a, strmat("  [ 0 0 0; 0 0 0 ]"));
        check_equal(-1*a, strmat("  [ -1 -2 -3; -4 -5 -6 ]"));
        
        Matrix empty;
        Matrix empty1;
        
        check_equal(empty, 1*empty1);
        check_equal(empty, 0*empty1);
    }
    
    void testMinus() {
        Matrix a =         strmat("  [ 1 2 3 ; 4 5 6 ; 7 8 9 ]");
        Matrix b = matrix_3by3(); // [ 3 1 4 ; 1 5 9 ; 0 0 0 ]
        
        Matrix c = a - b;
        Matrix res = strmat("  [ -2 1 -1; 3 0 -3; 7 8 9 ]");
        check_equal(c, res);
        
        Matrix e = a_matrix_3by2();
        TS_ASSERT_THROWS_ANYTHING(e - b);
        TS_ASSERT_THROWS_ANYTHING(b - e);
    }
    
    void testNegation() {
        Matrix a = matrix_3by3(); // [ 3 1 4 ; 1 5 9 ; 0 0 0 ]
        Matrix res = strmat("  [ -3 -1 -4 ; -1 -5 -9 ; 0 0 0 ]");
        check_equal(-a, res);
        
        Matrix empty, empty1;
        check_equal(empty, -empty1);
    }
    
    void testTranspose() {
        Matrix a = matrix_3by3(); // [ 3 1 4 ; 1 5 9 ; 0 0 0 ]
        Matrix res = strmat("  [ 3 1 0 ; 1 5 0 ; 4 9 0 ]");
        check_equal(a.transpose(), res);
        check_equal(a, res); // should modify object that it was called on
        
        Matrix b = strmat("  [ 1 2 3 ]");
        Matrix c = strmat("  [ 1; 2; 3 ]");
        check_equal(b, c.transpose());
        check_equal(b, c); // should modify object that it was called on
        
        Matrix empty, empty1;
        check_equal(empty.transpose(), empty1);
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

