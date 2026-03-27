#ifndef ULTRASONICCONTROLLER_H
#define ULTRASONICCONTROLLER_H

#include "sensors/interfaces/IUltraSonicSensor.h"
#include "types/Block.h"
#include "types/Codes.h"

using namespace Types;
using namespace Codes;

class UltraSonicController
{

private:
    IUltraSonicSensor *ultraSonic;

public:
    int8_t init(IUltraSonicSensor *ultraSonic);

    // Perform a full measurement and return distance in cm
    UltraSonicResult readDistanceCm();

    // Check if an object is within a certain distance threshold in cm
    UltraSonicWithinResult isThereObjectWithin(uint16_t thresholdCm);
};

#endif
