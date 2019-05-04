#ifndef COMMON_H
#define COMMON_H

#include "log.cpp"
#include <cerrno>
#include <ctime>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>
#ifndef LOG
#define LOG(...) log_std_print(__FILE__, __func__, __LINE__, __VA_ARGS__);
#endif
#ifndef LOG_ERR
#define LOG_ERR(...)                                                           \
  log_stderr_print(__FILE__, __func__, __LINE__, __VA_ARGS__);
#endif

#endif // COMMON_H
