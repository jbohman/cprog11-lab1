#include <cstddef>
#include <cassert>
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
         * Behaves like >, but implemented using <.
         */
        static bool greater_with_less(const T & a, const T & b) {
            return b < a;
        }

    public:
        /**
         * Default constructor
         */
        Vector() :
            capacity(2),
            internal_size(0),
            vector(new T[capacity]) { }
        
        /**
         * Copy constructor
         */
        Vector(const Vector & copy) :
            capacity(copy.internal_size),
            internal_size(copy.internal_size),
            vector(new T[capacity])
       {
            for (size_t i = 0; i < internal_size; ++i) {
                vector[i] = copy.vector[i];
            }
        }

        explicit Vector(size_t size) :
            capacity(size + 1),
            internal_size(size),
            vector(new T[capacity]()) // () = value initilization
        {
        }

        explicit Vector(size_t size, const T & default_value) :
            capacity(size + 1),
            internal_size(size),
            vector(new T[capacity])
        {
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
                capacity = copy.internal_size; // we only allocated this size!
                internal_size = copy.internal_size;
            }
            return *this;
        }

        T & operator[](size_t index) {
            // size_t is unsigned so index >= 0
            if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }

        const T & operator[](size_t index) const {
            if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];
        }

        /**
         * Push back
         */
        void push_back(const T & element) {
            if (internal_size >= capacity) {
                update_capacity();
            }
            vector[internal_size] = element;
            internal_size++;
        }

        /**
         * Insert
         */
        void insert(size_t index, const T & element) {
            if (index > internal_size) {
                throw std::out_of_range("out of range");
            } else if (index == internal_size) {
                push_back(element);
            } else {
                if (internal_size >= capacity) {
                    update_capacity();
                }
                for (size_t i = internal_size; i > index; --i) {
                    vector[i] = vector[i-1];
                }
                vector[index] = element;
                internal_size++;
            }
        }

        /**
         * Erase
         */
        void erase(size_t index) {
            if (index >= internal_size) {
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

        /**
         * Resize. New elements are assigned the given value.
         */
        void resize(size_t new_size, const T& value = T()) {
            if (new_size > capacity) {
                // Need to resize
                capacity = new_size;
                T * new_vector = new T[capacity];
                for (size_t i = 0; i < internal_size; ++i) {
                    new_vector[i] = vector[i];
                }
                delete [] vector;
                vector = new_vector;

            }

            if (new_size > internal_size) {
                // Set the new elements to the default value
                for (size_t i = internal_size; i < new_size; ++i) {
                    vector[i] = value;
                }
            }

            internal_size = new_size;
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
                // a custom comparer which uses <.
                std::sort(vector, vector + internal_size, greater_with_less);
            }
        }

    private:
        /**
         * Update capacity
         */
        void update_capacity() {
            if (capacity == 0) capacity++;
            capacity = capacity * 2;
            assert(capacity > internal_size);
            
            T * new_vector = new T[capacity];
            for (size_t i = 0; i < internal_size; ++i) {
                new_vector[i] = vector[i];
            }
            delete [] vector;
            vector = new_vector;
        }
};

#endif
