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
        
        b[0][1] = 31415;
        TS_ASSERT_EQUALS(b[0][1], 31415);
        TS_ASSERT_EQUALS(a[0][1], 3);
        
        Matrix c;
        Matrix d = c;
        check_equal(c, d);
    }
    
    void testAssign()
    {
        // Test assign 3x2 to 1x1
        Matrix a = a_matrix_3by2();
        Matrix b(1, 1);
        b = a;
        check_equal(a, b);
        check_string(a, "[ 1 3 5 \n; 0 2 0 ]");
        check_string(b, "[ 1 3 5 \n; 0 2 0 ]");
        
        b[0][0] = 2;
        check_not_equal(a, b);
        check_string(a, "[ 1 3 5 \n; 0 2 0 ]");
        check_string(b, "[ 2 3 5 \n; 0 2 0 ]");
        a[0][0] = 2;
        check_equal(a, b);
        
        // Test assign 0x0 to 1x4
        Matrix c;
        Matrix d(1, 4);
        d = c;
        check_equal(c, d);
        
        // Test assign 3x5 to 0x0
        Matrix e(3, 5);
        Matrix f;
        f = e;
        check_equal(e, f);
        
        f[0][0] = 2;
        check_not_equal(e, f);
        e[0][0] = 2;
        check_equal(e, f);
        
        // Test assign square to square
        Matrix g(2), h(2);
        g[0][1] = 3;
        h[1][0] = 5;
        check_not_equal(g, h);
        g = h;
        check_equal(g, h);
        
        g[0][0] = 2;
        check_not_equal(g, h);
        h[0][0] = 2;
        check_equal(g, h);
    }
    
    void testAssignTranspose()
    {
        static const size_t num_tests = 4;
        static const char* tests[num_tests] = {
            "[ 5 ]", "[ 7 11 ]", "[ 13 \n; 17 ]", "[ 19 23 \n; 29 31 ]"
        };
        static const char* transposed[num_tests] = {
            "[ 5 ]", "[ 7 \n; 11 ]", "[ 13 17 ]", "[ 19 29 \n; 23 31 ]"
        };
        
        for (size_t i = 0; i < num_tests; i++) {
            Matrix m = strmat(tests[i]);
            Matrix n = m;
            
            n.transpose();
            
            check_string(m, tests[i]);
            check_string(n, transposed[i]);
            
            m.transpose();
            
            check_string(m, transposed[i]);
            check_string(n, transposed[i]);
            
            m = n;
            
            n.transpose();
            
            check_string(m, transposed[i]);
            check_string(n, tests[i]);
        }
    }
    
    void testAssignRow()
    {
        Matrix a(2), b(2);
        a[0][1] = 3;
        
        b[1] = a[0];
        TS_ASSERT_EQUALS(b[0][0], 1);
        TS_ASSERT_EQUALS(b[0][1], 0);
        TS_ASSERT_EQUALS(b[1][0], 1);
        TS_ASSERT_EQUALS(b[1][1], 3);
        
        a[0][0] = 5;
        TS_ASSERT_EQUALS(b[1][0], 1);
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
        
        Matrix res1 = strmat("  [ 2 4 6; 8 10 12 ]");
        check_equal(2*a, res1);
        check_equal(a*2, res1);
        
        Matrix res2 = strmat("[ 0 0 0; 0 0 0 ]");
        check_equal(0*a, res2);
        check_equal(a*0, res2);
        
        Matrix res3 = strmat("[ -1 -2 -3; -4 -5 -6 ]");
        check_equal(-1*a, res3);
        check_equal(a*-1, res3);
        
        Matrix empty;
        Matrix empty1;
        
        check_equal(empty, 1*empty1);
        check_equal(empty, 0*empty1);
        check_equal(empty, empty1*1);
        check_equal(empty, empty1*0);
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
    
    void testPrint() {
        Matrix a = matrix_3by3(); // [ 3 1 4 ; 1 5 9 ; 0 0 0 ]
        Matrix b = strmat("  [ 3 1 0 ; 1 5 0 ]");
        Matrix v = strmat("  [ 3 1 4 ]");
        Matrix u = strmat("  [ 1; 5; 9 ]");
        Matrix z;
        Matrix id(2);
        Matrix sq(2);
        sq[0][1] = 2;
        sq[1][0] = 7;
        
        // Copy
        const int count = 7;
        Matrix* originals[count] = { &a, &b, &v, &u, &z, &id, &sq };
        Matrix copies[count] = { a, b, v, u, z, id, sq };
        
        check_string(a, "[ 3 1 4 \n; 1 5 9 \n; 0 0 0 ]");
        check_string(b, "[ 3 1 0 \n; 1 5 0 ]");
        check_string(v, "[ 3 1 4 ]");
        check_string(u, "[ 1 \n; 5 \n; 9 ]");
        //check_string(z, "[ ]"); // freezes in all implementations
        check_string(id, "[ 1 0 \n; 0 1 ]");
        check_string(sq, "[ 1 2 \n; 7 1 ]");
        
        // Check that the matrices are unmodified
        for (int i = 0; i < count; i++) {
            check_equal(*originals[i], copies[i]);
        }
        
        // Manual check
        TS_ASSERT_EQUALS(sq[0][0], 1);
        TS_ASSERT_EQUALS(sq[0][1], 2);
        TS_ASSERT_EQUALS(sq[1][0], 7);
        TS_ASSERT_EQUALS(sq[1][1], 1);
    }
    
    void testScanning() {
        Matrix a = strmat("[ 1 ; 2 ]");
        Matrix b = strmat("[ 1 ;\n 2 ]");
        Matrix c = strmat("[1;2]");
        Matrix d = strmat("    [1;2]");
        Matrix e(2,1);
        e[0][0] = 1;
        e[1][0] = 2;
        
        check_equal(a, e);
        check_equal(b, e);
        check_equal(c, e);
        check_equal(d, e);
        
        Matrix f = strmat("[]");
        Matrix empty;
        check_equal(f, empty);
        
        Matrix g = strmat("[1]");
        Matrix scalar(1);
        check_equal(g, scalar);
        
        Matrix h = strmat("[1 2; 3 4]");
        Matrix square(2);
        square[0][0] = 1;
        square[0][1] = 2;
        square[1][0] = 3;
        square[1][1] = 4;
        check_equal(h, square);
    }
    
    void testChained() {
    	Matrix a = strmat("[ 3 5; 7 11 ]");
    	Matrix b(2);
    	Matrix c = strmat("[ 13 17; 23 21 ]");
    	Matrix d = strmat("[ 1; 2 ]");
    	
    	Matrix res = (a+b)*(c*d);
    	check_string(res, "[ 513 \n; 1109 ]");
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
    
    void check_string(/*const*/ Matrix & m, const char *str)
    {
        for (size_t i = 0; i < 2; ++i) {
            std::stringstream ss;
            ss << m;
            TS_ASSERT_EQUALS(ss.str(), str);
        }
    }
};

#endif

