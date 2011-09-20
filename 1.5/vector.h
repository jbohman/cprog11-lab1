#include <cstddef>
#include <stdexcept>

#ifndef VECTOR_H
#define VECTOR_H

template <class T> class Vector {
    private:
        size_t size;
        T * vector;
    public:
        Vector(const Vector & copy) {
            size = copy.size;
            vector = new int[size];
            for (unsigned int i = 0; i < size; ++i) {
                vector[i] = copy.vector[i];
            }
        }

        explicit Vector(size_t size) {
            Vector::size = size;
            vector = new T[size];
            for (unsigned int i = 0; i < size; ++i) {
                vector[i] = 0;
            }
        }
        
        ~Vector() {
            delete[] vector;
        }

        Vector & operator=(const Vector & copy) {
            if (this != &copy) {
                // 1: allocate new memory and copy the elements
                T * new_vector = new T[copy.size];
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

        T & operator[](const unsigned int & index) {
            if (index < 0 || index >= size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }

        T operator[](const unsigned int & index) const {
            if (index < 0 || index >= size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }
};

#endif
