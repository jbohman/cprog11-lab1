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
        
        size_t required_size(size_t num_bits) {
            return (num_bits + bits_per_int - 1) / bits_per_int;
        }
        
        size_t which_byte(size_t bit) {
            return bit / bits_per_int;
        }
        
        size_t which_bit(size_t bit) {
            return bit % bits_per_int;
        }
    
    public:
    
        class bitproxy {
                
                elemtype & ref;
                size_t bitindex;
                
                bitproxy(elemtype & element, size_t bit) :
                    ref(element),
                    bitindex(bit) { }
                
                friend class Vector<bool>;
                
            public:
                bitproxy & operator=(bool value) {
                    ref = (ref & ~(1 << bitindex)) | (ref << bitindex);
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
            cout << "which byte:" << which_byte(index) << "   bitindex:" << which_bit(index) << endl;
            return bitproxy(data[which_byte(index)], which_bit(index));
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

