#include "UltraSonicController.h"

int8_t UltraSonicController::init(IUltraSonicSensor *ultraSonic)
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

uint16_t UltraSonicController::readDistanceCm()
{
    //! NOTE: occurs over multiple frames, NEED TO CHANGE LOGIC TO OCCUR OVER MULTIPLE FRAMES
    // Need to change writeTrigger to separate functions
    ultraSonic->writeTrigger();

    float duration = ultraSonic->readEchoTimeUs();

    if (duration == 0)
    {
        // No echo received (out of range)
        return 0;
    }

    // Convert microseconds to centimeters
    // distance_cm = duration / 58
    return static_cast<uint16_t>((duration * .0343) / 2);
}

bool UltraSonicController::isThereObjectWithin(uint16_t thresholdCm)
{
    uint16_t distance = readDistanceCm();

    if (distance == 0)
    {
        return false; // No object detected
    }

    return distance <= thresholdCm;
}