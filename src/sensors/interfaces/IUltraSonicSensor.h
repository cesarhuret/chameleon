#ifndef IULTRASONICSENSOR_H
#define IULTRASONICSENSOR_H

#include <stdint.h>

class IUltraSonicSensor {
public:
    virtual ~IUltraSonicSensor() = default;

    virtual int8_t init() = 0;

    // Perform full measurement and return distance in cm
    virtual uint16_t readDistanceCm() = 0;

    virtual bool isObjectWithin(uint16_t thresholdCm) = 0;
};

#endif