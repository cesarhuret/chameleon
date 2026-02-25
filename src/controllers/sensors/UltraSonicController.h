#ifndef ULTRASONICCONTROLLER_H
#define ULTRASONICCONTROLLER_H

#include "sensors/interfaces/IUltraSonicSensor.h"
#include "types/Block.h"

using namespace Types;

class UltraSonicController
{

private:
    IUltraSonicSensor* ultraSonic;

public:
    int8_t init(IUltraSonicSensor* ultraSonic);

};

#endif
