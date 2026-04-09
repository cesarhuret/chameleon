#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>
#include <stdint.h>

#include "sensors/interfaces/IUltraSonicSensor.h"
#include "types/Codes.h"
#include "types/Config.h"

using namespace Codes;


class UltraSonicSensor : public IUltraSonicSensor
{
public:

    uint8_t init(uint8_t trigPin, uint8_t echoPin) override;

    void writeTrigger();
    float readEchoTimeUs();

private:
    uint8_t _trigPin;
    uint8_t _echoPin;
};

#endif
