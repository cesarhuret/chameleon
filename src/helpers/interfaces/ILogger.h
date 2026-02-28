#ifndef ILOGGER_H
#define ILOGGER_H

#include <stdarg.h>

enum class LogLevel {
    Debug = 0,
    Info,
    Error,
    None
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(LogLevel level, const char* format, va_list args) = 0;

    void debug(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::Debug, format, args);
        va_end(args);
    }

    void info(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::Info, format, args);
        va_end(args);
    }

    void error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::Error, format, args);
        va_end(args);
    }
};

#endif