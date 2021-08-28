//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_LOG_H
#define ANGEMON_LOG_H

#include "headers.h"

namespace angemon {


#ifndef LOG
#define LOG(...) log_std_print(__FILE__, __func__, __LINE__, __VA_ARGS__);
#endif
#ifndef LOG_ERR
#define LOG_ERR(...)                                                           \
log_stderr_print(__FILE__, __func__, __LINE__, __VA_ARGS__);
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG(...) log_std_print(__FILE__, __func__, __LINE__, __VA_ARGS__);
#endif
#ifndef log_message
#define log_message(level, ...) log_message_print(level, __FILE__, __func__, __LINE__, __VA_ARGS__)
#endif

    enum LogLevel {
        kLogLevelError = 0,
        kLogLevelWarning = 1,
        kLogLevelInfo = 2,
        kLogLevelDebug = 3,
    };

    void log_message_print(LogLevel lv, const char *filename, const char *func, int line,
                           const char *fmt, ...);

    void log_stderr_print(const char *filename, const char *func, int line,
                          const char *fmt, ...);

    void log_std_print(const char *filename, const char *func, int line,
                       const char *fmt, ...);


}


#endif //ANGEMON_LOG_H
