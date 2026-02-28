#ifndef IULTRASONICSENSOR_H
#define IULTRASONICSENSOR_H

#include <stdint.h>

class IUltraSonicSensor
{
public:
    virtual ~IUltraSonicSensor() = default;

    virtual int8_t init() = 0;

    virtual void writeTrigger() = 0;
    virtual float readEchoTimeUs() = 0;
};

#endif