#include <cstddef>

#ifndef VECTOR_H
#define VECTOR_H

class Vector {
    private:
        size_t size;
        unsigned int * vector;
    public:
        Vector(const Vector &);
        explicit Vector(size_t size);
        ~Vector();
        Vector & operator=(const Vector &);
        unsigned int & operator[](const unsigned int & index);
        unsigned int operator[](const unsigned int & index) const;
};

#endif
