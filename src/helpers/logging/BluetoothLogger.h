#ifndef BLUETOOTHLOGGER_H
#define BLUETOOTHLOGGER_H

#include "helpers/interfaces/ILogger.h"
#include <Arduino.h>

constexpr uint8_t MAX_PAYLOAD_SIZE = 10;
constexpr uint8_t BT_HANDSHAKE_BYTE = 0x7E;
constexpr uint8_t BT_HANDSHAKE_ACK_BYTE = 0xAC;

class BluetoothLogger : public ILogger
{
private:
    HardwareSerial &uart;
    LogLevel minimumLevel;
    unsigned long baudRate;
    int8_t statePin;
    uint8_t connectedState;

    uint8_t waitForHandshake(unsigned long timeoutMs)
    {
        const unsigned long start = millis();

        while (true)
        {
            while (uart.available() > 0)
            {
                const int incoming = uart.read();
                if (incoming == BT_HANDSHAKE_BYTE)
                {
                    uart.write(BT_HANDSHAKE_ACK_BYTE);
                    return 0;
                }
            }

            if (timeoutMs > 0 && (millis() - start) >= timeoutMs)
                return 1;

            delay(20);
        }
    }

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

        uart.write(packet, 4 + size);
    }

public:
    BluetoothLogger(HardwareSerial &serialPort = Serial,
                    LogLevel level = LogLevel::Debug,
                    unsigned long baud = 57600,
                    int8_t btStatePin = -1,
                    uint8_t btConnectedState = HIGH)
        : uart(serialPort), minimumLevel(level), baudRate(baud), statePin(btStatePin), connectedState(btConnectedState) {}

    uint8_t init(unsigned long timeoutMs = 0)
    {
        uart.begin(baudRate);
        
        // HC-06 on TX/RX-only wiring has no link-state signal, so use UART handshake.
        return waitForHandshake(timeoutMs);
    }

    void setLevel(LogLevel level) { minimumLevel = level; }

    template <typename T>
    void log(LogLevel level, const T &data, uint8_t type)
    {
        logImpl(level, &data, sizeof(T), type);
    }
    
};

#endif