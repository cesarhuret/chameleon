#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include "helpers/interfaces/ILogger.h"
#include <Arduino.h>

class SerialLogger : public ILogger
{
private:
    LogLevel minimumLevel;

public:
    SerialLogger(LogLevel level = LogLevel::Debug, int baudRate = 115200)
        : minimumLevel(level)
    {
    }

    void setLevel(LogLevel level)
    {
        minimumLevel = level;
    }

    void log(LogLevel level, const char *format, va_list args) override
    {
        if (level < minimumLevel)
            return;

        char buffer[128]; // adjust size if needed
        vsnprintf(buffer, sizeof(buffer), format, args);

        switch (level)
        {
        case LogLevel::Debug:
            Serial.print("[DEBUG] [");
            break;
        case LogLevel::Info:
            Serial.print("[INFO] [");
            break;
        case LogLevel::Error:
            Serial.print("[ERROR] [");
            break;
        default:
            break;
        }

        Serial.print(millis());
        Serial.print("ms] : ");

        Serial.println(buffer);
    }
};

#endif