//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_THREAD_OBJECT_H
#define ANGEMON_THREAD_OBJECT_H


#include "headers.h"

namespace angemon {
    template<typename T, uint16_t N>
    class ThreadObject {
    public:
        ThreadObject();

        T *GetMainObject();

        T *GetObject(uint16_t idx);

        uint16_t Size() const { return N; }

    private:
        const uint16_t _size;
        T *_objects;
    };


    template<typename T, uint16_t N>
    ThreadObject<T, N>::ThreadObject():_size(N) {
        // first of object is main
        _objects = new T[N + 1];
    }

    template<typename T, uint16_t N>
    T *ThreadObject<T, N>::GetMainObject() {
        return &_objects[0];
    }

    template<typename T, uint16_t N>
    T *ThreadObject<T, N>::GetObject(uint16_t idx) {
        if (N < 1) {
            return GetMainObject();
        }
        return &_objects[idx % N + 1];
    }

}


#endif //ANGEMON_THREAD_OBJECT_H
