//
// Created by tiyee on 2021/8/25.
//

#include "bytes_buffer.h"

angemon::BytesBuffer::BytesBuffer() : _m_buffer(nullptr), _pos(0), _size(0), _capacity(0) {

}

angemon::BytesBuffer::BytesBuffer(size_t cap) : _m_buffer(nullptr), _pos(0), _size(0), _capacity(cap) {
    if (cap > 0) {
        Extend(cap);
    }
}

angemon::BytesBuffer::~BytesBuffer() {
    printf("~BytesBuffer pos=%ld, size=%ld cap=%ld \n", _pos, _size, _capacity);
    Clear();
    if (_m_buffer != nullptr) {
        free(_m_buffer);
        _m_buffer = nullptr;
    }
}

void angemon::BytesBuffer::Extend(uint32_t len) {
    size_t alloc_size = 4; //default 4 byte;
    while (alloc_size < 1024 && alloc_size < _capacity + len) {
        alloc_size *= 2;
    }
    while (alloc_size < _capacity + len) {
        alloc_size += (alloc_size >> 2);  // increase by 1/4 allocate size
    }
    if (_m_buffer == nullptr) {
        _m_buffer = (uchar_t *) malloc(alloc_size);
    } else {
        _m_buffer = (uchar_t *) realloc(_m_buffer, alloc_size);
    }
    _capacity = alloc_size;
}

size_t angemon::BytesBuffer::Write(const uchar_t *buf, uint32_t len) {
    if (_m_buffer == nullptr) {
        Extend(len);
    } else if (_size + len > _capacity) {
        Extend(_size + len - _capacity);
    }
    ::memcpy(_m_buffer + _size, buf, len);
    _size += len;
    return _size;
}

size_t angemon::BytesBuffer::Read(uchar_t *&buf, size_t len) {
    if (_pos + len > _size) {
        return 0;
    }
    buf = (uchar_t *) malloc(len);
    ::memcpy(buf, _m_buffer + _pos, len);
    _pos += len;
    return len;
}

void angemon::BytesBuffer::Clear() {
    Reset();
    _size = 0;
}

void angemon::BytesBuffer::Reset() {
    _pos = 0;
}

angemon::BytesBuffer::BytesBuffer(uchar_t *buff, size_t len) : _m_buffer(buff), _pos(0), _size(len), _capacity(len) {


}

void angemon::BytesBuffer::ReadLastAll(uchar_t *&buff) {
    auto size = _size - _pos;
    buff = (uchar_t *) malloc(size);
    Read(buff, size);

}
