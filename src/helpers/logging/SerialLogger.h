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

    static const char *levelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO ";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    static const char *codeToString(uint8_t code)
    {
        switch (code)
        {
        case Codes::SUCCESS:
            return "SUCCESS";
        case Codes::SETUP_SUCCESS:
            return "SETUP_SUCCESS";
        case Codes::INIT_FAILURE:
            return "INIT_FAILURE";
        case Codes::RUNTIME_ERROR:
            return "RUNTIME_ERROR";
        case Codes::PIXY_INITIALISING:
            return "PIXY_INITIALISING";
        case Codes::PIXY_NOT_AVAILABLE:
            return "PIXY_NOT_AVAILABLE";
        case Codes::PIXY_INVALID_INDEX:
            return "PIXY_INVALID_INDEX";
        case Codes::PIXY_BLOCK_NOT_FOUND:
            return "PIXY_BLOCK_NOT_FOUND";
        case Codes::PIXY_TARGET_NOT_FOUND:
            return "PIXY_TARGET_NOT_FOUND";
        case 0xFF:
            return "PIXY_RESULT_ERROR";
        case 0xFE:
            return "PIXY_RESULT_BUSY";
        case 0xFD:
            return "PIXY_RESULT_CHECKSUM_ERROR";
        case 0xFC:
            return "PIXY_RESULT_TIMEOUT";
        case 0xFB:
            return "PIXY_RESULT_BUTTON_OVERRIDE";
        case 0xFA:
            return "PIXY_RESULT_PROG_CHANGING";
        case Codes::ULTRASONIC_NOT_AVAILABLE:
            return "ULTRASONIC_NOT_AVAILABLE";
        case Codes::ULTRASONIC_OUT_OF_RANGE:
            return "ULTRASONIC_OUT_OF_RANGE";
        case Codes::HALT:
            return "HALT";
        default:
            return "UNKNOWN_CODE";
        }
    }

    static void printHexByte(uint8_t value)
    {
        if (value < 0x10)
            Serial.print('0');
        Serial.print(value, HEX);
    }

    void printDecodedPayload(uint8_t type, const uint8_t *bytes, size_t size)
    {
        if (size == 0)
        {
            Serial.print("-");
            return;
        }

        if (type == 0 && size == 1)
        {
            const uint8_t code = bytes[0];
            Serial.print(codeToString(code));
            Serial.print(" (0x");
            printHexByte(code);
            Serial.print(")");
            return;
        }

        if (type == 2 && size == 1)
        {
            const uint8_t stateValue = bytes[0];
            const Types::State state = static_cast<Types::State>(stateValue);
            Serial.print(Types::toString(state));
            Serial.print(" (");
            Serial.print(stateValue);
            Serial.print(")");
            return;
        }

        if (type == 3 && size == sizeof(Types::PackedBlock))
        {
            Types::PackedBlock block{};
            memcpy(&block, bytes, sizeof(block));
            Serial.print("x=");
            Serial.print(block.x);
            Serial.print(" y=");
            Serial.print(block.y);
            Serial.print(" w=");
            Serial.print(block.width);
            Serial.print(" h=");
            Serial.print(block.height);
            Serial.print(" sig=");
            Serial.print(block.signature);
            Serial.print(" age=");
            Serial.print(block.age);
            return;
        }

        if (size == 2)
        {
            int16_t value = 0;
            memcpy(&value, bytes, sizeof(value));
            Serial.print(value);
            return;
        }

        if (size == 1)
        {
            Serial.print(bytes[0]);
            return;
        }

        if (size == 4)
        {
            int32_t signedValue = 0;
            uint32_t unsignedValue = 0;
            memcpy(&signedValue, bytes, sizeof(signedValue));
            memcpy(&unsignedValue, bytes, sizeof(unsignedValue));
            Serial.print("s32=");
            Serial.print(signedValue);
            Serial.print(" u32=");
            Serial.print(unsignedValue);
            return;
        }

        for (size_t i = 0; i < size; i++)
        {
            if (i > 0)
                Serial.print(' ');
            printHexByte(bytes[i]);
        }
    }

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

protected:
    void logImpl(LogLevel level, const void *data, size_t size, uint8_t type) override
    {
        if (level < minimumLevel)
            return;

        Serial.print("[");
        Serial.print(levelToString(level));
        Serial.print("] Type:");
        Serial.print(type);
        Serial.print(" Size:");
        Serial.print(size);
        Serial.print(" Raw:");

        const uint8_t *bytes = static_cast<const uint8_t *>(data);
        for (size_t i = 0; i < size; i++)
        {
            if (i > 0)
                Serial.print(" ");
            printHexByte(bytes[i]);
        }

        Serial.print(" Decoded:");
        printDecodedPayload(type, bytes, size);
        Serial.println();
    }
};

#endif