#include "OurMatrix.h"
#include <cctype>

Matrix::Matrix() :
   m_vectors(0),
   m_rows(0),
   m_cols(0)
{
}

Matrix::Matrix(size_t rows, size_t cols) :
    m_vectors(rows, matrix_row(cols)), // initial value is copied to each row
    m_rows(rows),
    m_cols(cols)
{
/*    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            
        }
    }*/
}

Matrix::Matrix(const Matrix& m) :
    m_vectors(m.rows(), matrix_row(m.cols())),
    m_rows(m.rows()),
    m_cols(m.cols())
{
    for (size_t r = 0; r < m_rows; ++r) {
        for (size_t c = 0; c < m_cols; ++c) {
            (*this)[r][c] = m[r][c];
        }
    }
}

Matrix::Matrix(size_t size) :
    m_vectors(size, matrix_row(size)),
    m_rows(size),
    m_cols(size)
{
    // Create identity matrix
    for (size_t i = 0; i < size; ++i) {
        (*this)[i][i] = 1;
    }
}

Matrix::~Matrix()
{
    //delete m_vectors;
}

/*    class matrix_row : private Vector< int >
    {
    public:
        matrix_row( std::size_t s = 0) : Vector< int >( s ) {}
        using Vector<int>::operator [];
    private:
        friend std::istream& operator>>( std::istream&, Matrix& );
    };*/

size_t Matrix::rows() const { return m_rows; }
size_t Matrix::cols() const { return m_cols; }

Matrix& Matrix::operator=(const Matrix& m)
{
    if (&m != this) {
        // Resize if size if different
        size_t mr = m.rows();
        size_t mc = m.cols();
        if (mr != m_rows || mc != m_cols) {
            m_vectors.clear();
            m_vectors.resize(mr, matrix_row(mc));
            m_rows = mr;
            m_cols = mc;
        }
        
        // Copy elements
        for (size_t r = 0; r < mr; ++r) {
            for (size_t c = 0; c < mc; ++c) {
                (*this)[r][c] = m[r][c];
            }
        }
    }
    return *this;
}

void Matrix::check_same_size(const Matrix& m) const
{
    if (m.rows() != m_rows || m.cols() != m_cols) {
        throw std::logic_error("matrices must have the same size");
    }
}

Matrix Matrix::operator+(const Matrix& m) const
{
    check_same_size(m);
    
    Matrix res(*this);
    for (size_t r = 0; r < m_rows; ++r) {
        for (size_t c = 0; c < m_cols; ++c) {
            res[r][c] += m[r][c];
        }
    }
    return res;
}

Matrix Matrix::operator*(const Matrix& m) const
{
    size_t mr = m.rows();
    size_t mc = m.cols();
    
    if (m_cols != mr) {
        throw std::logic_error("these matrices cannot be multiplied");
    }

    Matrix res(m_rows, mc);
    // For each result cell
    for (size_t r = 0; r < m_rows; ++r) {
        for (size_t c = 0; c < mc; ++c) {
            // Calculate vector product
            int sum = 0;
            for (size_t i = 0; i < mr; ++i) {
                sum += (*this)[r][i] * m[i][c];
            }
            res[r][c] = sum;
        }
    }
    return res;
}

Matrix Matrix::operator*(int factor) const
{
    Matrix res(*this);
    for (size_t r = 0; r < m_rows; ++r) {
        for (size_t c = 0; c < m_cols; ++c) {
            res[r][c] *= factor;
        }
    }
    return res;
}

Matrix Matrix::operator-(const Matrix& m) const
{
    check_same_size(m);
    
    Matrix res(*this);
    for (size_t r = 0; r < m_rows; ++r) {
        for (size_t c = 0; c < m_cols; ++c) {
            res[r][c] -= m[r][c];
        }
    }
    return res;
}

Matrix Matrix::operator-() const
{
    return *this * -1;
}

Matrix& Matrix::transpose()
{
    Matrix copy = *this;
    
    size_t mr = m_cols;
    size_t mc = m_rows;
    // Resize if needed
    if (mr != mr) {
        m_vectors.clear();
        m_vectors.resize(mr, matrix_row(mc));
        m_rows = mr;
        m_cols = mc;
    }
    
    // Assign transposed
    for (size_t r = 0; r < mr; ++r) {
        for (size_t c = 0; c < mc; ++c) {
            (*this)[r][c] = copy[c][r];
        }
    }
    
    return *this;
}

Matrix::matrix_row& Matrix::operator[](index i)
{
    return m_vectors[i];
}

const Matrix::matrix_row& Matrix::operator[](index i) const
{
    return m_vectors[i];
}

static void skip_space(std::istream& is)
{
    while (is.good()) {
        if (!std::isspace(is.get())) {
            is.unget();
            break;
        }
    }
}

std::istream& operator>>(std::istream& is, Matrix& m)
{
    // Read [
    skip_space(is);
    if (!is.good() || is.get() != '[') {
        throw std::invalid_argument("does not start with [");
    }
    skip_space(is);
    
    size_t c = 0, r = 0;; // current column / row
    size_t cols = -1; // number of columns
    Matrix::matrix_row values;
    while (is.good()) {
        int ch = is.get();
        
        // Ignore whitespace
        if (std::isspace(ch)) continue;
        
        if (ch == ';' || ch == ']') {
            // Set / check column count
            if (r == 0 && c == 0 && ch == ']') {
                // Empty matrix
                m = Matrix();
                return is;
            } else if (r == 0) {
                // First row
                cols = c;
                m = Matrix(1, cols);
                m.m_vectors.clear(); // remove zero row
            } else if (c != cols) {
                // Not same length as first row
                throw std::invalid_argument("rows have different lengths");
            } else {
                ++m.m_rows;
            }
            
            // Add this row
            m.m_vectors.push_back(values);
            values.clear();
            c = 0;
            ++r;
            
            if (ch == ']') return is;
            else continue;
        }
        
        // Number
        is.unget();
        int num;
        is >> num;
        values.push_back(num);
        ++c;
    }
    
    throw std::invalid_argument("unexpected end of stream");
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    size_t rows = m.rows(), cols = m.cols();
    
    os << "[ ";
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            os << m[r][c] << " ";
        }
        
        if (r != rows-1) {
            os << "\n; ";
        }
    }
    os << "]";
    return os;
}


Matrix operator*(int factor, const Matrix& m)
{
    // Commutative operation
    return m * factor;
}


/* Deprecated functions - For backwards compatibility */
Matrix::Matrix(int size) :
    m_vectors(size, matrix_row(size)),
    m_rows(size),
    m_cols(size)
{
    // Create identity matrix
    for (int i = 0; i < size; ++i) {
        (*this)[i][i] = 1;
    }
}

std::ostream& operator<<(std::ostream& os, Matrix& m)
{
    return os << static_cast<const Matrix&>(m);
}

