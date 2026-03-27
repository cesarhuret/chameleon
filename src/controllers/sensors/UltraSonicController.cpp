#include "UltraSonicController.h"

#include <Arduino.h>


int8_t UltraSonicController::init(IUltraSonicSensor *ultraSonic)
{
    this->ultraSonic = ultraSonic;

    if (this->ultraSonic == nullptr)
    {
        return ULTRASONIC_NOT_AVAILABLE;
    }

    uint8_t status = this->ultraSonic->init();
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
    return {SUCCESS, static_cast<uint16_t>((duration * .0343) / 2)};
}

UltraSonicWithinResult UltraSonicController::isThereObjectWithin(uint16_t thresholdCm)
{
    UltraSonicResult result = readDistanceCm();
    if (result.status != SUCCESS)
    {
        return {result.status, false}; // Return error status
    }

    if (result.distanceCm == 0)
    {
        return {ULTRASONIC_OUT_OF_RANGE, false}; // No object detected
    }

    Serial.print("Checking if object is within ");
    Serial.print(thresholdCm);
    Serial.print(" cm: ");
    Serial.print(result.distanceCm);
    Serial.println(" cm");

    return {SUCCESS, result.distanceCm <= thresholdCm}; // Return whether object is within threshold
}