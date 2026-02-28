#ifndef ULTRASONICCONTROLLER_H
#define ULTRASONICCONTROLLER_H

#include "sensors/interfaces/IUltraSonicSensor.h"
#include "types/Block.h"

using namespace Types;

class UltraSonicController
{

private:
    IUltraSonicSensor *ultraSonic;

public:
    int8_t init(IUltraSonicSensor *ultraSonic);

    // Perform a full measurement and return distance in cm
    uint16_t readDistanceCm();

    // Check if an object is within a certain distance threshold in cm
    bool isThereObjectWithin(uint16_t thresholdCm);
};

#endif
