#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>

using namespace std;

#include "../1.5/vector.h"

template <>
class Vector<bool> {

        typedef unsigned int elemtype;

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
                    ref = (ref & ~(1 << bitindex)) | (value << bitindex);
                    return *this;
                }
                
                bitproxy & operator=(const bitproxy & proxy) {
                    *this = static_cast<bool>(proxy);
                    return *this;
                }
                
                operator bool () const {
                    return (ref >> bitindex) & 1;
                }
        };
        
        class const_iterator : public std::iterator<std::random_access_iterator_tag, bool> {
                
                const Vector<bool> *vector;
                size_t index;
                
                void check_same(const const_iterator & iter) const {
                    if (iter.vector != vector) {
                        throw std::logic_error("comparing iterators on different vectors");
                    }
                }
                
            protected:
                const_iterator(const Vector<bool> & v, size_t i) :
                    vector(&v),
                    index(i) { }
                
                friend class Vector<bool>;
                
            public:
                
                const_iterator(const const_iterator & other) :
                    vector(other.vector),
                    index(other.index) { }
                
                const_iterator& operator++() { index++; return *this; }
                const_iterator operator++(int) {
                    const_iterator copy(*this);
                    copy++;
                    return copy;
                }
                
                bool operator==(const const_iterator& other) const {
                    return vector == other.vector &&
                        index == other.index;
                }
                
                bool operator!=(const const_iterator& other) const {
                    return !(*this == other);
                }
                
                bool operator*() { return (*vector)[index]; }
                
                size_t operator-(const const_iterator & b) const {
                    check_same(b);
                    return index - b.index;
                }
                
                const_iterator & operator+=(const size_t & amount) {
                    index += amount;
                    return *this;
                }
        };
        
        const_iterator begin() const {
            return const_iterator(*this, 0);
        }
        
        const_iterator end() const {
            return const_iterator(*this, length);
        }

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
         * Not
         */
        Vector<bool> operator ~() const {
            Vector<bool> result(*this);
            for(size_t i = 0; i < result.data.size(); i++) {
                result.data[i] ^= all_bits;
            }
            return result;
        }

        /**
         * And
         */
        Vector<bool> operator &(const Vector<bool> & other) const {
            if (length != other.length) {
                throw std::logic_error("doing bit-operations on bitvectors of different length");
            }
            Vector<bool> result(*this);
            for(size_t i = 0; i < result.data.size(); i++) {
                result.data[i] &= other.data[i];
            }
            return result;
        }

         /**
         * Or
         */
        Vector<bool> operator |(const Vector<bool> & other) const {
            if (length != other.length) {
                throw std::logic_error("doing bit-operations on bitvectors of different length");
            }
            Vector<bool> result(*this);
            for(size_t i = 0; i < result.data.size(); i++) {
                result.data[i] |= other.data[i];
            }
            return result;
        }

        /**
         * Xor
         */
        Vector<bool> operator ^(const Vector<bool> & other) const {
            if (length != other.length) {
                throw std::logic_error("doing bit-operations on bitvectors of different length");
            }
            Vector<bool> result(*this);
            for(size_t i = 0; i < result.data.size(); i++) {
                result.data[i] ^= other.data[i];
            }
            return result;
        }
        /**
         * Push back
         */
        void push_back(bool element) {
            length++;
            if (length % bits_per_int == 1) {
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
            // Resize the integer array
            data.resize(required_size(new_size), value ? all_bits : 0);
            
            if (length && new_size > length) {
                // Set bits in the last bits in the old int array
                // (not assigned by resize)
                elemtype temp = data[which_byte(length-1)];
                for (size_t i = which_bit(length); i < bits_per_int; ++i) {
                    temp = (temp & ~(1 << i)) | (value << i);
                }
                data[which_byte(length-1)] = temp;
            }
            
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
            size_t num_first = 0;
            size_t i;
            for (i = 0; i < length; ++i) {
                if ((*this)[i] != ascending) ++num_first;
            }
            
            // Set first values
            for (i = 0; i < num_first; ++i) {
                (*this)[i] = !ascending;
            }
            
            // Set last values
            for (; i < length; ++i) {
                (*this)[i] = ascending;
            }
        }
        
        /**
         * Returns the bitvector as an integer value, or throws
         * std::logic_error if the bit vector is too large.
         */
        elemtype get_int() {
            if (length > bits_per_int) {
                throw std::logic_error("bitvector is larger than an int");
            } else if (length == 0) {
                return 0;
            }
            
            // Clear unused bits
            elemtype filtered = data[0];
            for (size_t i = length; i < bits_per_int; ++i) {
                filtered = (filtered & ~(1 << i));
            }
            return filtered;
        }

        /**
         * Weight, mathemtically
         */
        unsigned long weight_math() {
            unsigned long result = 0;
            unsigned long x_count = 0;
            for (size_t i = 0; i < data.size() - 1; i++) {
                x_count = data[i] - ((data[i] >> 1) & 033333333333) - ((data[i] >> 2) & 011111111111);
                result += ((x_count + (x_count >> 3)) & 030707070707) % 63;
            }

            elemtype last_data = data[data.size() - 1];
            last_data <<= (data.size() * bits_per_int) - length;
            x_count = last_data - ((last_data >> 1) & 033333333333) - ((last_data >> 2) & 011111111111);
            result += ((x_count + (x_count >> 3)) & 030707070707) % 63;

            return result;
        }

        /**
         * Weight, 256 bit lookup array
         */
        unsigned long weight_lookup() {
            static unsigned char bits_lookup[256];
            bits_lookup[0] = 0;
            for (int i = 0; i < 256; i++) {
                bits_lookup[i] = (i & 1) + bits_lookup[i / 2];
            }

            unsigned long result = 0;
            for (size_t i = 0; i < data.size() - 1; i++) {
                result += bits_lookup[data[i] & 0xff] + bits_lookup[(data[i] >> 8) & 0xff] +
                          bits_lookup[(data[i] >> 16) & 0xff] + bits_lookup[(data[i] >> 24) & 0xff];
            }

            elemtype last_data = data[data.size() - 1];
            last_data <<= (data.size() * bits_per_int) - length;
            result += bits_lookup[last_data & 0xff] + bits_lookup[(last_data >> 8) & 0xff] +
                      bits_lookup[(last_data >> 16) & 0xff] + bits_lookup[(last_data >> 24) & 0xff];

            return result;
        }

        /**
         * Weight, loop
         */
        unsigned long weight_loop() {
            unsigned long result = 0;
            //for (size_t i = 0; i < data.size(); i++) {
                //unsigned long temp = data[i];
                //for (; temp; temp >>= 1) {
                    //result += temp & 1;
                //}
            //}
            for (size_t i = 0; i < length; ++i) {
                result += (*this)[i] & 1;
            }
            return result;
        }
        
        /* Iterator traits */
        typedef bool value_type;
        typedef size_t difference_type;
        typedef bool* pointer;
        typedef bool& reference;
        typedef std::random_access_iterator_tag iterator_category;
};

#endif
