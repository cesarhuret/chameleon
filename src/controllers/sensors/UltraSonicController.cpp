#include "UltraSonicController.h"

#include <Arduino.h>
#include "types/Config.h"


int8_t UltraSonicController::init(IUltraSonicSensor *ultraSonic, uint8_t trigPin, uint8_t echoPin)
{
    this->ultraSonic = ultraSonic;

    if (this->ultraSonic == nullptr)
    {
        return ULTRASONIC_NOT_AVAILABLE;
    }

    uint8_t status = this->ultraSonic->init(trigPin, echoPin);
    return status;
}

UltraSonicResult UltraSonicController::readDistanceCm()
{
    //! NOTE: occurs over multiple frames, NEED TO CHANGE LOGIC TO OCCUR OVER MULTIPLE FRAMES
    // Need to change writeTrigger to separate functions
    ultraSonic->writeTrigger();

    float duration = ultraSonic->readEchoTimeUs();

    if (duration == 0)
    {
        // No echo received (out of range)
        return {ULTRASONIC_OUT_OF_RANGE, 0};
    }

    // Convert microseconds to centimeters
    // distance_cm = duration / 58

    lastDistanceCm = static_cast<uint16_t>((duration * .0343) / 2); // Speed of sound is ~343 m/s, so 0.0343 cm/µs, and we divide by 2 for the round trip

    return {SUCCESS, lastDistanceCm};
}

UltraSonicWithinResult UltraSonicController::isThereObjectWithin(uint16_t thresholdCm)
{

    if (lastDistanceCm == 0)
    {
        return {ULTRASONIC_OUT_OF_RANGE, false}; // No object detected
    }


    return {SUCCESS, lastDistanceCm <= thresholdCm}; // Return whether object is within threshold
}