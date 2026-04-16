#include "controllers/main/Chameleon.h"
#include "SPI.h"

#include "sensors/arduino/PixySensor.h"
#include "sensors/arduino/UltraSonicSensor.h"
#include "controllers/actuators/ServoController.h"
#include "helpers/logging/BluetoothLogger.h"
#include "helpers/logging/SerialLogger.h"
#include "types/Codes.h"
#include "types/Config.h"

constexpr unsigned long BAUD_RATE = 115200UL;

Chameleon chameleon;
SerialLogger logger(LogLevel::Debug, BAUD_RATE);
PixySensor pixySensor;
UltraSonicSensor bottomUltrasoundSensor;
UltraSonicSensor topUltrasoundSensor;
// BluetoothLogger logger(Serial, LogLevel::Debug, BAUD_RATE); // uses 0 and 1
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
void setup()
{

    Serial.begin(BAUD_RATE);

    SPI.begin();

    RuntimeConfig config = {
        .pixy = &pixySensor,
        .bottomUltraSonic = &bottomUltrasoundSensor,
        .topUltraSonic = &topUltrasoundSensor
    };
    
    Serial.println(freeMemory());

    uint8_t status = chameleon.init(&logger, config);
    if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        halt(); // Halt on initialization failure
    }

    // logger.log(LogLevel::Info, Codes::SETUP_SUCCESS, 0);
    return;
}

void loop()
{

    // logger.log(LogLevel::Info, freeMemory(), 0);

    uint8_t status = chameleon.run();
    if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        halt(); // Halt on runtime error
    }
}

// Halt function to stop execution in case of any failure
void halt()
{
    while (true)
    {
        logger.log(LogLevel::Error, Codes::HALT, 0);
        delay(5000); // Log the error message every 5 seconds
    }
}
