#ifndef ROOMBA_H
#define ROOMBA_H

#include "helpers/interfaces/ILogger.h"
#include "sensors/interfaces/IPixySensor.h"
#include "sensors/interfaces/IUltraSonicSensor.h"
#include "actuators/arduino/LED.h"
#include "controllers/sensors/PixyController.h"
#include "controllers/sensors/UltraSonicController.h"
#include "controllers/actuators/MotorController.h"
#include "controllers/actuators/ServoController.h"
#include "types/State.h"

class Roomba
{

private:
    State currentState;
    State previousState;
    DetectedBlock lastValidTarget;

    ILogger *logger;
    PixyController pixyController;
    UltraSonicController bottomUltrasoundController;
    UltraSonicController topUltrasoundController;
    MotorController motorController;
    ServoController servoController;
    LED led;

public:
    // should we store a list of all the blocks that were scanned in that frame?
    // because when we get our target block we want its latest information - as we get closer the position will update
    // then we just use the index to fetch the latest information from the updated list

    uint8_t init(ILogger *logger, RuntimeConfig config);
    uint8_t run();
};

#endif