#include "UltraSonicSensor.h"


UltraSonicSensor::UltraSonicSensor(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin)
{
}

uint8_t UltraSonicSensor::init()
{
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);

    writeTrigger(); // Send initial trigger to ensure sensor is ready

    float duration = readEchoTimeUs(); // Read initial echo to clear any noise

    Serial.print("Initial ultrasonic sensor check, duration: ");
    Serial.println(duration);


    if (duration == 0)
    {
        // No echo received, sensor might not be working
        return ULTRASONIC_OUT_OF_RANGE;
    }

    return SUCCESS;
}

void UltraSonicSensor::writeTrigger()
{
    // Ensure clean LOW pulse
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);

    // 10µs HIGH pulse to trigger measurement
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(_trigPin, LOW);
}

float UltraSonicSensor::readEchoTimeUs()
{
    // Timeout at 30ms (~5m max distance)
    return pulseIn(_echoPin, HIGH);
}