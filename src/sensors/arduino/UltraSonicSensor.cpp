#include "UltraSonicSensor.h"

UltraSonicSensor::UltraSonicSensor(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin)
{
}

int8_t UltraSonicSensor::init()
{
    pinMode(3, OUTPUT);
    pinMode(2, INPUT);

    // digitalWrite(_trigPin, LOW); // Ensure trigger starts LOW
    return 0;                    // Success
}

void UltraSonicSensor::writeTrigger()
{
    // Ensure clean LOW pulse
    digitalWrite(3, LOW);
    delayMicroseconds(2); // DO NOT DO THIS IN PROD - USE A TIMER - THIS IS BLOCKING!

    // 10µs HIGH pulse to trigger measurement
    digitalWrite(3, HIGH);
    delayMicroseconds(10); // DO NOT DO THIS IN PROD - USE A TIMER - THIS IS BLOCKING!

    digitalWrite(3, LOW);
}

float UltraSonicSensor::readEchoTimeUs()
{
    // Timeout at 30ms (~5m max distance)
    return pulseIn(2, HIGH);
}