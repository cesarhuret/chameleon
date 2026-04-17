#ifndef RUNTIME_CONFIG_H
#define RUNTIME_CONFIG_H

#include <stdint.h>
#include "sensors/interfaces/IPixySensor.h"
#include "sensors/interfaces/IUltraSonicSensor.h"

struct RuntimeConfig {
    IPixySensor *pixy;
    IUltraSonicSensor *bottomUltraSonic;
    IUltraSonicSensor *topUltraSonic;
};

#endif