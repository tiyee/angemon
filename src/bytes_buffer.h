//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_BYTES_BUFFER_H
#define ANGEMON_BYTES_BUFFER_H

#include "headers.h"



namespace angemon {
    class BytesBuffer {
    public:
        BytesBuffer();

        explicit BytesBuffer(size_t cap);

        BytesBuffer(uchar_t *buff, size_t len);

        ~BytesBuffer();

        size_t Write(const uchar_t *buf, uint32_t len);

        size_t Read(uchar_t *&buf, size_t len);

        void Clear();

        void Reset();

        void ReadLastAll(uchar_t *buff);

        size_t GetWritableSize() const { return _capacity - _size; };

        size_t GetCapacity() const {
            return _capacity;
        };

        size_t GetSize() const {
            return _size;
        };

        size_t IncrSize(size_t n) {
            return _size += n;
        }

        size_t IncrPos(size_t n) {
            return _pos += n;
        }

        size_t GetPost() const {
            return _size;
        }

        bool IsReadable() const {
            return GetReadableSize() > 0;
        }

        void Extend(uint32_t len);

        uchar_t *GetWritableBuffer() const { return _m_buffer + _size; }

        size_t GetReadableSize() const {
            return _size - _pos;
        }

        uchar_t *GetReadableBuff() const {
            return _m_buffer + _pos;
        }

    private:


        uchar_t *_m_buffer;
        size_t _pos; // read position
        size_t _size; // write position
        size_t _capacity;
    };
}


#endif //ANGEMON_BYTES_BUFFER_H
