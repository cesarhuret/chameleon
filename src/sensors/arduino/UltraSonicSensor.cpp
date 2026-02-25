#include "UltraSonicSensor.h"

UltraSonicSensor::UltraSonicSensor(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin)
{
}

int8_t UltraSonicSensor::init()
{
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);

    digitalWrite(_trigPin, LOW); // Ensure trigger starts LOW
    return 0;                    // Success
}

void UltraSonicSensor::writeTrigger()
{
    // Ensure clean LOW pulse
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2); // DO NOT DO THIS IN PROD - USE A TIMER - THIS IS BLOCKING!

    // 10µs HIGH pulse to trigger measurement
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10); // DO NOT DO THIS IN PROD - USE A TIMER - THIS IS BLOCKING!

    digitalWrite(_trigPin, LOW);
}

uint32_t UltraSonicSensor::readEchoTimeUs()
{
    // Timeout at 30ms (~5m max distance)
    return pulseIn(_echoPin, HIGH, 30000);
}