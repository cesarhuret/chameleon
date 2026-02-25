#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>
#include <stdint.h>

#include "sensors/interfaces/IUltraSonicSensor.h"

class UltraSonicSensor : public IUltraSonicSensor
{
public:
    UltraSonicSensor(uint8_t trigPin, uint8_t echoPin);

    int8_t init() override;
    uint16_t readDistanceCm() override;
    bool isObjectWithin(uint16_t thresholdCm) override;

private:
    void writeTrigger();
    uint32_t readEchoTimeUs();

    uint8_t _trigPin;
    uint8_t _echoPin;
};

#endif
