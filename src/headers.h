//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_HEADERS_H
#define ANGEMON_HEADERS_H

#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>
#include <cerrno>
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cstdarg>
#ifdef __JEMALLOC__
#include <jemalloc/jemalloc.h>
#define malloc(size) je_malloc(size)
#define calloc(count, size) je_calloc(count,size)
#define realloc(ptr, size) je_realloc(ptr,size)
#define free(ptr) je_free(ptr)
#endif //__JEMALLOC__
typedef std::thread::id tid_t;
typedef unsigned char uchar_t;
#endif //ANGEMON_HEADERS_H
