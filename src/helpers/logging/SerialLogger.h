#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include "helpers/interfaces/ILogger.h"
#include "types/Block.h"
#include "types/Codes.h"
#include "types/State.h"
#include <Arduino.h>
#include <string.h>

class SerialLogger : public ILogger
{
private:
    LogLevel minimumLevel;

protected:
    void logImpl(LogLevel level, const void *data, size_t size, uint8_t type) override
    {
        if (level < minimumLevel)
            return;

        if (size > MAX_PAYLOAD_SIZE)
            size = MAX_PAYLOAD_SIZE;

        uint8_t packet[4 + MAX_PAYLOAD_SIZE];
        packet[0] = 0xAA;
        packet[1] = static_cast<uint8_t>(level);
        packet[2] = type;
        packet[3] = static_cast<uint8_t>(size);
        if (size > 0)
            memcpy(packet + 4, data, size);

        Serial.write(packet, 4 + size);
    };

public:
    SerialLogger(LogLevel level = LogLevel::Debug, unsigned long baudRate = 115200UL)
        : minimumLevel(level)
    {
        (void)baudRate;
    }

    void setLevel(LogLevel level)
    {
        minimumLevel = level;
    }


};

#endif