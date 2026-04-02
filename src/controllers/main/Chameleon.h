#ifndef CHAMELEON_H
#define CHAMELEON_H

#include "helpers/interfaces/ILogger.h"
#include "sensors/interfaces/IPixySensor.h"
#include "sensors/interfaces/IUltraSonicSensor.h"
#include "controllers/sensors/PixyController.h"
#include "controllers/sensors/UltraSonicController.h"
#include "controllers/actuators/MotorController.h"
#include "controllers/actuators/ServoController.h"
#include "types/State.h"



class Chameleon {

private:
    
    State currentState;
    State previousState;

    ILogger* logger;
    PixyController pixyController;
    UltraSonicController ultraSonicController;
    MotorController motorController;
    ServoController servoController;
    IPixySensor *pixy;
public:

    // should we store a list of all the blocks that were scanned in that frame?
    // because when we get our target block we want its latest information - as we get closer the position will update
    // then we just use the index to fetch the latest information from the updated list

    uint8_t init(ILogger* logger, IPixySensor* pixy, IUltraSonicSensor* ultraSonic);
    uint8_t run();
    
};


#endif