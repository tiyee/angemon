#include<ctime>
#include<cstdio>
#include <cstdarg>
void log_stderr_print(const char *filename, const char *func, int line,
                      const char *fmt, ...) {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    fprintf(stderr, "%4u-%u-%u %u:%u:%u", 1900 + ltm->tm_year, 1 + ltm->tm_mon,
            ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    fprintf(stderr, "[%s]:[%s]:[line %d]: ", filename, func, line);
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    fputc('\n', stderr);
}

void log_std_print(const char *filename, const char *func, int line,
                   const char *fmt, ...) {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    fprintf(stdout, "%4u-%u-%u %u:%u:%u", 1900 + ltm->tm_year, 1 + ltm->tm_mon,
            ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    fprintf(stdout, "[%s]:[%s]:[line %d]: ", filename, func, line);
    va_list list;
    va_start(list, fmt);
    vfprintf(stdout, fmt, list);
    va_end(list);
    fputc('\n', stdout);
}
