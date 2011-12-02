#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <functional>

#include <iostream>
using namespace std;


#include "../1.5/vector.h"

template <>
class Vector<bool> {

        typedef unsigned long elemtype;

        static const size_t bits_per_int = 32;
        static const elemtype all_bits = 0xFFFFFFFF;
        Vector<elemtype> data;
        size_t length;
        
        static size_t required_size(size_t num_bits) {
            return (num_bits + bits_per_int - 1) / bits_per_int;
        }
        
        size_t which_byte(size_t bit) const {
            return bit / bits_per_int;
        }
        
        size_t which_bit(size_t bit) const {
            return bit % bits_per_int;
        }
    
    public:
    
        class bitproxy {
                
                elemtype & ref;
                size_t bitindex;
                bitproxy();
                
           protected:
                bitproxy(elemtype & element, size_t bit) :
                    ref(element),
                    bitindex(bit) { }
                
                friend class Vector<bool>;
                
            public:
                bitproxy & operator=(bool value) {
                    ref = (ref & ~(1 << bitindex)) | (ref << value);
                    return *this;
                }
                
                operator bool () const {
                    return (ref >> bitindex) & 1;
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

        bitproxy operator[](size_t index) {
            // size_t is unsigned so index >= 0
            if (index >= length) {
                throw std::out_of_range("out of range");
            }
            return bitproxy(data[which_byte(index)], which_bit(index));
        }

        bool operator[](size_t index) const {
            if (index >= length) {
                throw std::out_of_range("out of range");
            }
            return (data[which_byte(index)] >> which_bit(index)) & 1;
        }

        /**
         * Push back
         */
        void push_back(bool element) {
            length++;
            if (length % bits_per_int == 0) {
                // grow int vector
                data.push_back(0);
            }
            
            (*this)[length-1] = element;
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
            data.resize(required_size(new_size));
            length = new_size;
        }

        void sort() {
            sort(true);
        }

        /**
         * Sort
         */
        void sort(bool ascending) {
            // Count number of ones
            size_t num_ones = 0;
            size_t i;
            for (i = 0; i < length; ++i) {
                if ((*this)[i]) ++num_ones;
            }
            
            // Set first values
            for (i = 0; i < num_ones; i++) {
                (*this)[i] = !ascending;
            }
            
            // Set last values
            for (; i < length; i++) {
                (*this)[i] = ascending;
            }
        }
        
};

#endif

