//
// Created by tiyee on 2021/8/25.
//

#include "log.h"

const std::string kLogLevelName[] = {"[ERROR]", "[WARNING]", "[INFO]", "[DEBUG]"};

void angemon::log_stderr_print(const char *filename, const char *func, int line,
                      const char *fmt, ...) {

    static std::mutex m;
    m.lock();

    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    fprintf(stderr, "%4u-%u-%u %u:%u:%u", 1900 + ltm->tm_year, 1 + ltm->tm_mon,
            ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    fprintf(stderr, "[%s]:[%s]:[line %d]", filename, func, line);
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    fputc('\n', stderr);
    m.unlock();
}

void angemon::log_std_print(const char *filename, const char *func, int line,
                   const char *fmt, ...) {
    static std::mutex m;
    m.lock();
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    fprintf(stderr, "%4u-%u-%u %u:%u:%u", 1900 + ltm->tm_year, 1 + ltm->tm_mon,
            ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    fprintf(stderr, "[%s]:[line %d]",  func, line);
    va_list list;
    va_start(list, fmt);
    vfprintf(stdout, fmt, list);
    va_end(list);
    fputc('\n', stdout);
    m.unlock();

}

void angemon::log_message_print(LogLevel lv, const char *filename, const char *func, int line,
                       const char *fmt, ...) {
    time_t now = time(nullptr);
    if (lv > kLogLevelDebug)return;
    tm *ltm = localtime(&now);
    fprintf(stdout, "%s %4u-%u-%u %u:%u:%u", kLogLevelName[lv].c_str(), 1900 + ltm->tm_year, 1 + ltm->tm_mon,
            ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    fprintf(stdout, "[%s]:[%s]:[line %d]: ", filename, func, line);
    va_list list;
    va_start(list, fmt);
    vfprintf(stdout, fmt, list);
    va_end(list);
    fputc('\n', stdout);
}