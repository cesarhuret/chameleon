#ifndef ILOGGER_H
#define ILOGGER_H

#include <stdint.h>

enum class LogLevel : uint8_t {
    Debug = 0,
    Info,
    Error,
    None
};

class ILogger {
public:
    virtual ~ILogger() = default;

    template <typename T>
    void log(LogLevel level, const T& data, uint8_t type) {
        logImpl(level, &data, sizeof(T), type);
    }

protected:
    virtual void logImpl(LogLevel level, const void* data, size_t size, uint8_t type) = 0;
};
#endif