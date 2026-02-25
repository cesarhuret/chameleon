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

    void writeTrigger();
    uint32_t readEchoTimeUs();

private:
    uint8_t _trigPin;
    uint8_t _echoPin;
};

#endif
