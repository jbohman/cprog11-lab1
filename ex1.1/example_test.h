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
    }

private:
    void check_zero(Matrix & m)
    {
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                TS_ASSERT_EQUALS(m[r][c], 0);
            }
        }
    }
    
    void check_identity(Matrix & m)
    {
        for (size_t r = 0; r < m.rows(); r++) {
            for (size_t c = 0; c < m.cols(); c++) {
                TS_ASSERT_EQUALS(m[r][c], r == c ? 1 : 0);
            }
        }
    }
};

#endif

