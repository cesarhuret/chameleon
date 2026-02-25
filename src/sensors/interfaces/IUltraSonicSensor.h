#ifndef IULTRASONICSENSOR_H
#define IULTRASONICSENSOR_H

#include <stdint.h>

class IUltraSonicSensor
{
public:
    virtual ~IUltraSonicSensor() = default;

    virtual int8_t init() = 0;

    virtual void writeTrigger() = 0;
    virtual uint32_t readEchoTimeUs() = 0;
};

#endif