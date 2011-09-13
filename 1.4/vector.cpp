#include <stdexcept>
#include "vector.h"

Vector::Vector(size_t size) {
    vector = new unsigned int[size];
    for (unsigned int i = 0; i < size; ++i) {
        vector[i] = 0;
    }
}

Vector::Vector(const Vector & copy) {
    size = copy.size;
    vector = new unsigned int[size];
    for (unsigned int i = 0; i < size; ++i) {
        vector[i] = copy.vector[i];
    }
}

Vector::~Vector() {
    delete[] vector;
}

Vector & Vector::operator=(const Vector & copy) {
    if (this != &copy) {
        // 1: allocate new memory and copy the elements
        unsigned int * new_vector = new unsigned int[copy.size];
        //std::copy(copy.vector, copy.vector + copy.size, new_vector);
        for (unsigned int i = 0; i < copy.size; ++i) {
            new_vector[i] = copy.vector[i];
        }

        // 2: deallocate old memory
        delete [] vector;

        // 3: assign the new memory to the object
        vector = new_vector;
        size = copy.size;
    }
    return *this;
}

unsigned int & Vector::operator[](const unsigned int & index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("out of range");
    }
    return vector[index];
}

unsigned int Vector::operator[](const unsigned int & index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("out of range");
    }
    return vector[index];
}
