/*
 * MIT License
 *
 * Copyright (c) 2020 Rynn Wu <liveinlow@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef INCLUDE_CUBE_H_
#define INCLUDE_CUBE_H_

#include <stddef.h>  // size_T

// A template class for creating a cube container to contain
// N x N x N elements with type T. The internal data arrangment is:
//
//          x
//    +------------+ +------------+     +------------+
//    |OX...       | |            |     |            |
// y  |            | |            | ... |V           |
//    |            | |            |     | W          |
//    |            | |            |     |            |
//    +------------+ +------------+     +------------+
//                                            z-th
// the character 'O' indicates to the (0, 0, 0) element.
// the character 'X' indicates to the (1, 0, 0) element.
// the character 'V' indicates to the (0, 1, z) element.
// the character 'W' indicates to the (1, 2, z) element.
//
// @tparam T: Type of elements in cube.
// @tparam SIDE: cube side count of element.
// @tparam ALIGNBYTES: internal buffer alignment, unit is bytes, padding at
// the end of buffer.
template<class T, size_t SIDE, size_t ALIGNBYTES = 16>
class Cube {
public:
    typedef T Type;  // helper typedef
    enum { TypeSize = sizeof(T) };  // helper constant of element size
    enum { Side = SIDE };  // helper constant of side element count
    enum { ElementCount = SIDE * SIDE * SIDE };  // element count
    enum { DataSize = ElementCount * TypeSize };  // data size in bytes
    enum { AlignBytes = ALIGNBYTES };  // helper constant of alignment bytes

public:
    // constructor
    Cube() {
        // compile time to calculate the bufSize
        constexpr size_t bufSize =
            (((ElementCount * TypeSize) + AlignBytes - 1) / AlignBytes)
            * AlignBytes;


        // create buffer chunk
        _buffer = new char[bufSize]{0};
        _bufferSize = bufSize;
    };

    virtual ~Cube() {
        if (_buffer) delete [] _buffer;  // release buffer if necessary
    }

public:

    // element access operator
    T& operator()(size_t x, size_t y, size_t z) {
        if (x >= Side || y >= Side || z >= Side) {
            // out of range.
            *(volatile int*)(0x0) = 0xdeadbeef;  // invoke exception
        }

        T* p = reinterpret_cast<T*>(_buffer);  // p points to start address of the buffer

        // move p to the position
        p += z * Side * Side;  // move to the z-th planes
        p += y * Side;  // move to the y-th lines
        p += x;  // move x steps
        return *p;
    }

    // get the raw data
    inline T* getData() { return reinterpret_cast<T*>(_buffer); }
    inline const T* getData() const { return reinterpret_cast<const T*>(_buffer); }

    // get the buffer
    inline void* getBuffer() { return _buffer; }
    inline const void* getBuffer() const { return _buffer; }

    // get the buffer size in bytes
    inline size_t getBufferSize() { return _bufferSize; }

public:
    char* _buffer;
    size_t _bufferSize;
};

#endif  // INCLUDE_CUBE_H
