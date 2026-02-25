#ifndef CHAMELEON_H
#define CHAMELEON_H

#include "sensors/interfaces/IPixySensor.h"
#include "sensors/interfaces/IUltraSonicSensor.h"
#include "controllers/sensors/PixyController.h"
#include "controllers/sensors/UltraSonicController.h"

enum class State {
    SEARCHING_FOR_BALL,
    MOVING_TO_BALL,
    CONTROLLING_CLAW,
    MOVING_TO_BASE
};

class Chameleon {

private:
    
    State currentState;

    PixyController pixyController;
    UltraSonicController ultraSonicController;
public:

    // should we store a list of all the blocks that were scanned in that frame?
    // because when we get our target block we want its latest information - as we get closer the position will update
    // then we just use the index to fetch the latest information from the updated list

    void init(IPixySensor* pixy, IUltraSonicSensor* ultraSonic);
    void run();
    
};


#endif