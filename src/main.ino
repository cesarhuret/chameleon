#include "controllers/main/Roomba.h"
#include "SPI.h"

#include "sensors/arduino/PixySensor.h"
#include "sensors/arduino/UltraSonicSensor.h"
#include "controllers/actuators/ServoController.h"
#include "helpers/logging/BluetoothLogger.h"
#include "helpers/logging/SerialLogger.h"
#include "types/Codes.h"
#include "types/Config.h"

constexpr unsigned long BAUD_RATE = 115200UL;

Roomba roomba;
SerialLogger logger(LogLevel::Debug, BAUD_RATE);
PixySensor pixySensor;
UltraSonicSensor bottomUltrasoundSensor;
UltraSonicSensor topUltrasoundSensor;
LED led;
// BluetoothLogger logger(Serial, LogLevel::Debug, BAUD_RATE); // uses 0 and 1
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory()
{
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
        .topUltraSonic = &topUltrasoundSensor};

    Serial.println(freeMemory());

    uint8_t status = roomba.init(&logger, config);
    if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        blink(led, false, true, false, 200); // Blink red if there is any other error
    }


    // logger.log(LogLevel::Info, Codes::SETUP_SUCCESS, 0);
    return;
}

void loop()
{

    uint8_t status = roomba.run();
    if (status == 55)
    {
        blink(led, true, true, false, 500); // Blink yellow if we fail to find the base for a long time, which might indicate an obstacle
    } else if(status == 66) {
        blink(led, false, false, true, 200); // Blink blue faster if we do see the base but it's not centered yet, which might indicate an obstacle
    }
    else if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        blink(led, true, false, false, 200); // Blink red if there is any other error
    }
}

void blink(LED &led, bool red, bool green, bool blue, unsigned long interval)
{
    while (true)
    {
        led.write(red, green, blue);
        delay(interval);
        led.write(false, false, false);
        delay(interval);
    }
}