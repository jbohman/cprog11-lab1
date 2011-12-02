#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <functional>

#include "../1.5/vector.h"

template <>
class Vector<bool> {

        typedef unsigned long elemtype;

        static const size_t bits_per_int = 32;
        static const elemtype all_bits = 0xFFFFFFFF;
        Vector<elemtype> data;
        size_t length;
        
        size_t required_size(size_t num_bits) {
            return (num_bits + bits_per_int - 1) / bits_per_int;
        }
    
    public:
    
        class bitproxy {
                
            public:
                bitproxy & operator=(bool value) {
                    // TODO
                    return *this;
                }
                
                operator bool () const {
                    // TODO
                    return false;
                }
        };

        /**
         * Default constructor
         */
        Vector() :
            data(0),
            length(0) { }
        
        /**
         * Copy constructor
         */
        Vector(const Vector & copy) :
            data(copy.data),
            length(copy.length) { }

        explicit Vector(size_t startsize) :
            data(required_size(startsize)),
            length(startsize) { }

        explicit Vector(size_t startsize, const bool default_value) :
            data(required_size(startsize),
                 default_value ? all_bits : 0),
            length(startsize) { }

        /**
         * Destructor
         */
        ~Vector() {
        }

        Vector & operator=(const Vector<bool> & copy) {
            if (this != &copy) {
                data = copy.data;
                length = copy.length;
            }
            return *this;
        }

        bitproxy & operator[](size_t index) {
            /*// size_t is unsigned so index >= 0
            if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];*/
        }

        const bitproxy operator[](size_t index) const {
            /*if (index >= internal_size) {
                throw std::out_of_range("out of range");
            }
            return vector[index];*/
        }

        /**
         * Push back
         */
        void push_back(bool element) {
            /*if (internal_size >= capacity) {
                update_capacity();
            }
            vector[internal_size] = element;
            internal_size++;*/
        }

        /**
         * Clear
         */
        void clear() {
            data.clear();
            length = 0;
        }

        /**
         * Size
         */
        size_t size() const {
            return length;
        }

        /**
         * Resize. New elements are assigned the given value.
         */
        void resize(size_t new_size, bool value = bool()) {
            // TODO
        }

        void sort() {
            sort(true);
        }

        /**
         * Sort
         */
        void sort(bool ascending) {
            // TODO
        }
        
};

#endif

