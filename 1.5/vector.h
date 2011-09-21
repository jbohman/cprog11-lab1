#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <functional>

#ifndef VECTOR_H
#define VECTOR_H

template <class T> class Vector {
    private:
        size_t capacity;
        size_t internal_size;
        T * vector;
        
        /**
         * This is simply the inverse of std::less
         */
        static bool not_less(const T & a, const T & b) {
            return !(a < b);
        }

    public:
        /**
         * Default constructor
         */
        Vector() {
            capacity = 2;
            internal_size = 0;
            vector = new T[capacity];
        }

        /**
         * Copy constructor
         */
        Vector(const Vector & copy) {
            capacity = copy.capacity;
            internal_size = copy.internal_size;
            vector = new T[capacity];
            for (size_t i = 0; i < internal_size; ++i) {
                vector[i] = copy.vector[i];
            }
        }

        explicit Vector(size_t size) {
            capacity =size + 1;
            internal_size = size;
            vector = new T[capacity];
            for (size_t i = 0; i < internal_size; ++i) {
                vector[i] = T();
            }
        }

        explicit Vector(size_t size, T default_value) {
            capacity = size + 1;
            internal_size = size;
            vector = new T[capacity];
            for (size_t i = 0; i < internal_size; ++i) {
                vector[i] = default_value;
            }
        }

        /**
         * Destructor
         */
        ~Vector() {
            delete[] vector;
        }

        Vector & operator=(const Vector & copy) {
            if (this != &copy) {
                // 1: allocate new memory and copy the elements
                T * new_vector = new T[copy.internal_size];
                //std::copy(copy.vector, copy.vector + copy.internal_size, new_vector);
                for (size_t i = 0; i < copy.internal_size; ++i) {
                    new_vector[i] = copy.vector[i];
                }

                // 2: deallocate old memory
                delete [] vector;

                // 3: assign the new memory to the object
                vector = new_vector;
                capacity = copy.capacity;
                internal_size = copy.internal_size;
            }
            return *this;
        }

        T & operator[](const size_t & index) {
            // size_t is unsigned so index >= 0
            if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }

        T operator[](const size_t & index) const {
            if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }

        /**
         * Push back
         */
        void push_back(T element) {
            if (internal_size >= capacity) {
                update_capacity();
            }
            vector[internal_size] = element;
            internal_size++;
        }

        /**
         * Insert
         */
        void insert(const size_t & index, T element) {
            if (index > internal_size) {
                throw std::out_of_range("out of range");
            } else if (index == internal_size) {
                push_back(element);
            } else {
                if (internal_size >= capacity) {
                    update_capacity();
                }
                for (size_t i = index; i < internal_size; ++i) {
                    vector[i+1] = vector[i];
                }
                vector[index] = element;
                internal_size++;
            }
        }

        /**
         * Erase
         */
        void erase(const size_t & index) {
            if (index > internal_size) {
                throw std::out_of_range("out of range");
            } else {
                internal_size--;
                for (size_t i = index; i < internal_size; ++i) {
                    vector[i] = vector[i+1];
                }
            }
        }

        /**
         * Clear
         */
        void clear() {
            internal_size = 0;
        }

        /**
         * Size
         */
        size_t size() const {
            return internal_size;
        }

        void sort() {
            sort(true);
        }

        /**
         * Sort
         */
        void sort(bool ascending) {
            if (ascending) {
                std::sort(vector, vector + internal_size, std::less<T>());
            } else {
                // We don't want to require T to implement > so we use
                // a custom comparer.
                std::sort(vector, vector + internal_size, not_less);
            }
        }

        /**
         * Update capacity
         */
        void update_capacity() {
            capacity = capacity * 2;
            T * new_vector = new T[capacity];
            for (size_t i = 0; i < internal_size; ++i) {
                new_vector[i] = vector[i];
            }
            delete [] vector;
            vector = new_vector;
        }
};

#endif
