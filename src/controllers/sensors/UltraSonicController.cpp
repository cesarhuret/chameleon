#include "UltraSonicController.h"

int8_t UltraSonicController::init(IUltraSonicSensor* ultraSonic)
{
    this->ultraSonic = ultraSonic;

    if (this->ultraSonic == nullptr)
    {
        return -1;
    }

    int8_t status = this->ultraSonic->init();
    if (status != 0)
    {
        // failed to initialize ultrasonic sensor, return error code
        return status;
    }
    return 0;
}